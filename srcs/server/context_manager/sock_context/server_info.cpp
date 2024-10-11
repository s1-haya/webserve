#include "server_info.hpp"
#include "define.hpp"

namespace server {

ServerInfo::ServerInfo() : fd_(-1), host_port_(std::make_pair(IPV4_ADDR_ANY, 0)) {}

ServerInfo::ServerInfo(const HostPortPair &host_port) : fd_(-1), host_port_(host_port) {}

ServerInfo::~ServerInfo() {}

ServerInfo::ServerInfo(const ServerInfo &other) {
	*this = other;
}

ServerInfo &ServerInfo::operator=(const ServerInfo &other) {
	if (this != &other) {
		fd_        = other.fd_;
		host_port_ = other.host_port_;
	}
	return *this;
}

int ServerInfo::GetFd() const {
	return fd_;
}

// const std::string &ServerInfo::GetHost() const {
// 	return host_port_.first;
// }

// unsigned int ServerInfo::GetPort() const {
// 	return host_port_.second;
// }

void ServerInfo::SetSockFd(int fd) {
	fd_ = fd;
}

} // namespace server
