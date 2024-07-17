#include "client_info.hpp"
#include <netdb.h>      // getnameinfo
#include <netinet/in.h> // struct sock_addr,INET6_ADDRSTRLEN,ntohs
#include <stdexcept>

namespace server {

ClientInfo::ClientInfo(int fd, const struct sockaddr_storage &sock_addr) : fd_(fd) {
	SetSockInfo(sock_addr);
}

ClientInfo::~ClientInfo() {
	// todo: close?
	// if (fd_ != SYSTEM_ERROR) {
	// 	close(fd_);
	// }
}

void ClientInfo::SetSockInfo(const struct sockaddr_storage &sock_addr) {
	// getnameinfo
	char      ip[INET6_ADDRSTRLEN];
	char      port[NI_MAXSERV];
	const int status = getnameinfo(
		(struct sockaddr *)&sock_addr,
		sizeof(sock_addr),
		ip,
		sizeof(ip),
		port,
		sizeof(port),
		NI_NUMERICHOST | NI_NUMERICSERV
	);
	if (status != 0) {
		// todo: tmp
		throw std::runtime_error("getnameinfo failed: " + std::string(gai_strerror(status)));
	}

	// set IP,port
	ip_   = ip;
	port_ = port;
}

const std::string &ClientInfo::GetIp() const {
	return ip_;
}

const std::string &ClientInfo::GetPort() const {
	return port_;
}

} // namespace server
