#include "client_info.hpp"
#include <netdb.h>      // getnameinfo
#include <netinet/in.h> // struct sock_addr,INET6_ADDRSTRLEN,ntohs
#include <stdexcept>

// todo: tmp
#include <iostream>

namespace server {

ClientInfo::ClientInfo(int fd, const struct sockaddr_storage &sock_addr) : fd_(fd), port_(0) {
	SetSockInfo(sock_addr);
	std::cout << "client (" << fd << ") " << "ip: " << ip_str_ << std::endl;
}

ClientInfo::~ClientInfo() {
	// todo: close?
	// if (fd_ != SYSTEM_ERROR) {
	// 	close(fd_);
	// }
}

void ClientInfo::SetSockInfo(const struct sockaddr_storage &sock_addr) {
	// getnameinfo
	char      ip_str[INET6_ADDRSTRLEN];
	const int status = getnameinfo(
		(struct sockaddr *)&sock_addr,
		sizeof(sock_addr),
		ip_str,
		sizeof(ip_str),
		NULL,
		0,
		NI_NUMERICHOST
	);
	if (status != 0) {
		// todo: tmp
		throw std::runtime_error("getnameinfo failed: " + std::string(gai_strerror(status)));
	}

	// set IP
	ip_str_ = ip_str;
}

} // namespace server
