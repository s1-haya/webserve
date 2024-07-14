#include "sock_info.hpp"

namespace server {

SockInfo::SockInfo() : fd_(0), name_(""), port_(0), peer_fd_(0) {}

SockInfo::SockInfo(const std::string &name, unsigned int port)
	: fd_(0), name_(name), port_(port), peer_fd_(0) {}

SockInfo::~SockInfo() {}

SockInfo::SockInfo(const SockInfo &other) {
	*this = other;
}

SockInfo &SockInfo::operator=(const SockInfo &other) {
	if (this != &other) {
		fd_      = other.fd_;
		name_    = other.name_;
		port_    = other.port_;
		peer_fd_ = other.peer_fd_;
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

void SockInfo::SetSockFd(int fd) {
	fd_ = fd;
}

void SockInfo::SetPeerSockFd(int fd) {
	peer_fd_ = fd;
}

} // namespace server
