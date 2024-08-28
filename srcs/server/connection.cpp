#include "connection.hpp"
#include "client_info.hpp"
#include "server.hpp"
#include "server_info.hpp"
#include "utils.hpp"    // ConvertUintToStr
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
	hints->ai_family   = AF_UNSPEC;
	hints->ai_flags    = AI_PASSIVE | AI_NUMERICSERV;
}

} // namespace

// result: dynamic allocated by getaddrinfo()
Connection::AddrInfo *Connection::GetAddrInfoList(const ServerInfo &server_info) {
	const std::string &port  = utils::ConvertUintToStr(server_info.GetPort());
	AddrInfo           hints = {};
	InitHints(&hints);

	AddrInfo *result;
	const int status = getaddrinfo(NULL, port.c_str(), &hints, &result);
	// EAI_MEMORY is also included in status != 0
	if (status != 0) {
		throw std::runtime_error("getaddrinfo failed: " + std::string(gai_strerror(status)));
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

int Connection::Connect(ServerInfo &server_info) {
	AddrInfo        *addrinfo_list = GetAddrInfoList(server_info);
	const BindResult bind_result   = TryBind(addrinfo_list);
	freeaddrinfo(addrinfo_list);
	if (!bind_result.IsOk()) {
		throw std::runtime_error("bind failed");
	}
	const int server_fd = bind_result.GetValue();

	// todo / listen() : set an appropriate backlog value
	// listen
	if (listen(server_fd, 3) == SYSTEM_ERROR) {
		close(server_fd);
		throw std::runtime_error("listen failed");
	}
	listen_server_fds_.insert(server_fd);

	return server_fd;
}

// todo: return ClientInfo *?
ClientInfo Connection::Accept(int server_fd) {
	struct sockaddr_storage client_sock_addr;
	socklen_t               addrlen = sizeof(client_sock_addr);
	const int client_fd = accept(server_fd, (struct sockaddr *)&client_sock_addr, &addrlen);
	if (client_fd == SYSTEM_ERROR) {
		throw std::runtime_error("accept failed");
	}

	// create new client struct
	ClientInfo client_info(client_fd, client_sock_addr);
	utils::Debug(
		"server", "new ClientInfo created. IP: " + client_info.GetIp() + ", fd", client_fd
	);
	return client_info;
}

bool Connection::IsListenServerFd(int sock_fd) const {
	return listen_server_fds_.count(sock_fd) == 1;
}

} // namespace server
