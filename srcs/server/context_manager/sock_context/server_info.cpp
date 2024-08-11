#include "server_info.hpp"

namespace server {

ServerInfo::ServerInfo() : fd_(0), port_(0) {}

ServerInfo::ServerInfo(unsigned int port) : fd_(0), port_(port) {}

ServerInfo::~ServerInfo() {}

ServerInfo::ServerInfo(const ServerInfo &other) {
	*this = other;
}

ServerInfo &ServerInfo::operator=(const ServerInfo &other) {
	if (this != &other) {
		fd_   = other.fd_;
		port_ = other.port_;
	}
	return *this;
}

int ServerInfo::GetFd() const {
	return fd_;
}

unsigned int ServerInfo::GetPort() const {
	return port_;
}

void ServerInfo::SetSockFd(int fd) {
	fd_ = fd;
}

} // namespace server
