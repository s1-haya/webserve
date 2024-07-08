#include "sock_info.hpp"
#include <arpa/inet.h> // htons
#include <cstring>     // memset

namespace server {

SockInfo::SockInfo() : fd_(0), name_(""), port_(0), addrlen_(sizeof(sock_addr_)), peer_fd_(0) {
	std::memset(&sock_addr_, 0, addrlen_);
}

SockInfo::SockInfo(const std::string &name, unsigned int port)
	: fd_(0), name_(name), port_(port), addrlen_(sizeof(sock_addr_)), peer_fd_(0) {
	sock_addr_.sin_family      = AF_INET;
	sock_addr_.sin_addr.s_addr = INADDR_ANY;
	sock_addr_.sin_port        = htons(port);
}

SockInfo::~SockInfo() {}

SockInfo::SockInfo(const SockInfo &other) {
	*this = other;
}

SockInfo &SockInfo::operator=(const SockInfo &other) {
	if (this != &other) {
		fd_        = other.fd_;
		name_      = other.name_;
		port_      = other.port_;
		sock_addr_ = other.sock_addr_;
		addrlen_   = other.addrlen_;
		peer_fd_   = other.peer_fd_;
	}
	return *this;
}

int SockInfo::GetFd() const {
	return fd_;
}

std::string SockInfo::GetName() const {
	return name_;
}

unsigned int SockInfo::GetPort() const {
	return port_;
}

struct sockaddr_in &SockInfo::GetSockAddr() {
	return sock_addr_;
}

socklen_t SockInfo::GetAddrlen() const {
	return addrlen_;
}

void SockInfo::SetSockFd(int fd) {
	fd_ = fd;
}

void SockInfo::SetPeerSockFd(int fd) {
	peer_fd_ = fd;
}

} // namespace server
