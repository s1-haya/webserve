#include "client_info.hpp"
#include <arpa/inet.h>  // ntohs
#include <netdb.h>      // getnameinfo
#include <netinet/in.h> // struct sock_addr,INET6_ADDRSTRLEN,ntohs
#include <stdexcept>

// todo: tmp
#include <iostream>

namespace server {

ClientInfo::ClientInfo(int fd, const struct sockaddr_storage &sock_addr) : fd_(fd), port_(0) {
	SetSockInfo(sock_addr);
	std::cout << "client (" << fd << ") " << "ip: " << ip_str_ << ", port: " << port_ << std::endl;
}

ClientInfo::~ClientInfo() {
	// todo: close?
	// if (fd_ != SYSTEM_ERROR) {
	// 	close(fd_);
	// }
}

namespace {

unsigned int ConvertPortToHostByteOrder(const struct sockaddr_storage &sock_addr) {
	// for IPv4
	if (sock_addr.ss_family == AF_INET) {
		return ntohs(((struct sockaddr_in *)&sock_addr)->sin_port);
	}
	// for IPv6
	if (sock_addr.ss_family == AF_INET6) {
		return ntohs(((struct sockaddr_in6 *)&sock_addr)->sin6_port);
	}
	// todo: tmp(unreachable?)
	return -1;
}

} // namespace

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

	// set IP,port
	ip_str_ = ip_str;
	port_   = ConvertPortToHostByteOrder(sock_addr);
}

} // namespace server
