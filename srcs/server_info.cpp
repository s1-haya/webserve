#include "server_info.hpp"

namespace server {

ServerInfo::ServerInfo() : fd_(0), name_(""), port_(0) {}

ServerInfo::ServerInfo(const std::string &name, unsigned int port)
	: fd_(0), name_(name), port_(port) {}

ServerInfo::~ServerInfo() {}

ServerInfo::ServerInfo(const ServerInfo &other) {
	*this = other;
}

ServerInfo &ServerInfo::operator=(const ServerInfo &other) {
	if (this != &other) {
		fd_   = other.fd_;
		name_ = other.name_;
		port_ = other.port_;
	}
	return *this;
}

int ServerInfo::GetFd() const {
	return fd_;
}

const std::string &ServerInfo::GetName() const {
	return name_;
}

unsigned int ServerInfo::GetPort() const {
	return port_;
}

void ServerInfo::SetSockFd(int fd) {
	fd_ = fd;
}

} // namespace server
