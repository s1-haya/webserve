#include "connection.hpp"
#include "client_info.hpp"
#include "server.hpp"
#include "server_info.hpp"
#include "system_exception.hpp"
#include "utils.hpp" // ConvertUintToStr
#include <cerrno>
#include <cstring>      // strerror
#include <netdb.h>      // getaddrinfo,freeaddrinfo
#include <netinet/in.h> // struct sockaddr
#include <stdexcept>    // runtime_error
#include <sys/socket.h> // socket,setsockopt,bind,listen,accept
#include <unistd.h>     // close

namespace server {

Connection::Connection() {}

Connection::~Connection() {}

namespace {

void InitHints(Connection::AddrInfo *hints) {
	hints->ai_socktype = SOCK_STREAM;
	hints->ai_family   = AF_INET; // IPv4
	hints->ai_flags    = AI_PASSIVE | AI_NUMERICSERV;
}

// 32bit(4bytes) -> xxx:xxx:xxx:xxxx
// struct in_addr addr = {0xc0, 0xa8, 0x01, 0x01}
// -> 192.168.1.1
std::string ConvertToIpv4Str(const struct in_addr &addr) {
	const uint32_t ip_addr = ntohl(addr.s_addr);

	std::ostringstream oss;
	for (int i = 24; i >= 0; i -= 8) {
		oss << ((ip_addr >> i) & 0xff);
		if (i != 0) {
			oss << '.';
		}
	}
	return oss.str();
}

// note: unused
// 128bit(16bytes) -> xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx
// struct in6_addr addr = {0x20, 0x01, 0x0d, 0xb8, 0x85, 0xa3, 0x00, 0x00,
//                         0x00, 0x00, 0x8a, 0x2e, 0x03, 0x70, 0x73, 0x34}
// -> 2001:db8:85a3:0:0:8a2e:370:7334
std::string ConvertToIpv6Str(const struct in6_addr &addr) {
	const uint8_t *bytes = addr.s6_addr;

	std::ostringstream oss;
	oss << std::hex;
	for (unsigned int i = 0; i < 16; i += 2) {
		if (i != 0) {
			oss << ':';
		}
		oss << ((bytes[i] << 8) | bytes[i + 1]);
	}
	oss << std::dec;
	return oss.str();
}

} // namespace

// "localhost" -> IpList{"127.0.0.1", other..} (no duplicates)
Connection::IpList Connection::ResolveHostName(const std::string &hostname) {
	AddrInfo hints = {};
	InitHints(&hints);

	AddrInfo *result = NULL;
	const int status = getaddrinfo(hostname.c_str(), NULL, &hints, &result);
	if (status != 0) {
		throw SystemException("getaddrinfo failed: " + std::string(gai_strerror(status)));
	}

	AddrInfo *head_result = result;
	// Temporary std::set for checking duplicate IPs.
	std::set<std::string> ip_set;
	for (; result != NULL; result = result->ai_next) {
		struct sockaddr_in *sa_in = (struct sockaddr_in *)result->ai_addr;
		const std::string   ip    = ConvertToIpv4Str(sa_in->sin_addr);
		ip_set.insert(ip);
	}
	freeaddrinfo(head_result);
	return IpList(ip_set.begin(), ip_set.end());
}

// result: dynamic allocated by getaddrinfo()
Connection::AddrInfo *Connection::GetAddrInfoList(const HostPortPair &host_port) const {
	const std::string &port  = utils::ConvertUintToStr(host_port.second);
	AddrInfo           hints = {};
	InitHints(&hints);

	AddrInfo *result = NULL;
	const int status = getaddrinfo(host_port.first.c_str(), port.c_str(), &hints, &result);
	// EAI_MEMORY is also included in status != 0
	if (status != 0) {
		throw SystemException("getaddrinfo failed: " + std::string(gai_strerror(status)));
	}
	return result;
}

Connection::BindResult Connection::TryBind(AddrInfo *addrinfo) const {
	BindResult bind_result(false, -1);

	for (; addrinfo != NULL; addrinfo = addrinfo->ai_next) {
		// socket
		const int server_fd =
			socket(addrinfo->ai_family, addrinfo->ai_socktype, addrinfo->ai_protocol);
		if (server_fd == SYSTEM_ERROR) {
			continue;
		}
		// set socket option to reuse address
		// optval : Specify a non-zero value to enable the boolean option, or zero to disable it
		int optval = 1;
		if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) ==
			SYSTEM_ERROR) {
			close(server_fd);
			continue;
		}

		// bind
		if (bind(server_fd, addrinfo->ai_addr, addrinfo->ai_addrlen) == SYSTEM_ERROR) {
			close(server_fd);
			continue;
		}
		// bind success
		bind_result.Set(true, server_fd);
		break;
	}
	return bind_result;
}

Connection::ListenResult Connection::Listen(int server_fd) {
	ListenResult listen_result;
	if (listen(server_fd, LISTEN_BACKLOG) == SYSTEM_ERROR) {
		close(server_fd);
		listen_result.Set(false);
		return listen_result;
	}
	listen_server_fds_.insert(server_fd);
	return listen_result;
}

int Connection::Connect(const HostPortPair &host_port) {
	AddrInfo        *addrinfo_list = GetAddrInfoList(host_port);
	const BindResult bind_result   = TryBind(addrinfo_list);
	freeaddrinfo(addrinfo_list);
	if (!bind_result.IsOk()) {
		throw SystemException("bind failed: " + std::string(std::strerror(errno)));
	}

	const int          server_fd     = bind_result.GetValue();
	const ListenResult listen_result = Listen(server_fd);
	if (!listen_result.IsOk()) {
		throw std::runtime_error("listen failed");
	}
	return server_fd;
}

Connection::IpPortPair Connection::GetIpPort(struct sockaddr_storage &sock_addr) {
	std::string  ip;
	unsigned int port = 0;
	if (sock_addr.ss_family == AF_INET) {
		struct sockaddr_in *sa_in = (struct sockaddr_in *)&sock_addr;
		ip                        = ConvertToIpv4Str(sa_in->sin_addr);
		port                      = ntohs(sa_in->sin_port);
	} else if (sock_addr.ss_family == AF_INET6) {
		struct sockaddr_in6 *sa_in6 = (struct sockaddr_in6 *)&sock_addr;
		ip                          = ConvertToIpv6Str(sa_in6->sin6_addr);
		port                        = ntohs(sa_in6->sin6_port);
	} else {
		throw std::runtime_error("GetIpPort: invalid ss_family");
	}
	return std::make_pair(ip, port);
}

Connection::IpPortPair Connection::GetListenServerIpPort(int client_fd) {
	struct sockaddr_storage listen_sock_addr     = {};
	socklen_t               listen_sock_addr_len = sizeof(listen_sock_addr);
	if (getsockname(client_fd, (struct sockaddr *)&listen_sock_addr, &listen_sock_addr_len) ==
		SYSTEM_ERROR) {
		throw SystemException("getsockname failed: " + std::string(std::strerror(errno)));
	}
	return GetIpPort(listen_sock_addr);
}

ClientInfo Connection::Accept(int server_fd) {
	struct sockaddr_storage client_sock_addr = {};
	socklen_t               addrlen          = sizeof(client_sock_addr);
	const int client_fd = accept(server_fd, (struct sockaddr *)&client_sock_addr, &addrlen);
	if (client_fd == SYSTEM_ERROR) {
		throw SystemException("accept failed: " + std::string(std::strerror(errno)));
	}

	// client
	const IpPortPair client_ip_port = GetIpPort(client_sock_addr);

	// listen server
	const IpPortPair   listen_ip_port = GetListenServerIpPort(client_fd);
	const std::string &listen_ip      = listen_ip_port.first;
	const unsigned int listen_port    = listen_ip_port.second;

	// create new client struct
	ClientInfo client_info(client_fd, client_ip_port.first, listen_ip, listen_port);
	utils::Debug("server", "new ClientInfo created. fd", client_fd);
	return client_info;
}

bool Connection::IsListenServerFd(int sock_fd) const {
	return listen_server_fds_.count(sock_fd) == 1;
}

} // namespace server
