#include "client_info.hpp"
#include <netdb.h>      // getnameinfo
#include <netinet/in.h> // struct sock_addr,INET6_ADDRSTRLEN,ntohs
#include <stdexcept>

namespace server {

ClientInfo::ClientInfo() : fd_(0) {}

ClientInfo::ClientInfo(int fd, const struct sockaddr_storage &sock_addr) : fd_(fd) {
	SetSockInfo(sock_addr);
}

ClientInfo::~ClientInfo() {
	// todo: close?
	// if (fd_ != SYSTEM_ERROR) {
	// 	close(fd_);
	// }
}

ClientInfo::ClientInfo(const ClientInfo &other) {
	*this = other;
}

ClientInfo &ClientInfo::operator=(const ClientInfo &other) {
	if (this != &other) {
		fd_ = other.fd_;
		ip_ = other.ip_;
	}
	return *this;
}

void ClientInfo::SetSockInfo(const struct sockaddr_storage &sock_addr) {
	// getnameinfo
	char      ip[INET6_ADDRSTRLEN];
	const int status = getnameinfo(
		(struct sockaddr *)&sock_addr,
		sizeof(sock_addr),
		ip,
		sizeof(ip),
		NULL,
		0,
		NI_NUMERICHOST | NI_NUMERICSERV
	);
	if (status != 0) {
		throw std::runtime_error("getnameinfo failed: " + std::string(gai_strerror(status)));
	}
	ip_ = ip;
}

int ClientInfo::GetFd() const {
	return fd_;
}

const std::string &ClientInfo::GetIp() const {
	return ip_;
}

} // namespace server
