#include "client_info.hpp"

namespace server {

ClientInfo::ClientInfo() : fd_(0), listen_port_(0) {}

ClientInfo::ClientInfo(int fd, const std::string &listen_ip, unsigned int listen_port)
	: fd_(fd), listen_ip_(listen_ip), listen_port_(listen_port) {}

ClientInfo::~ClientInfo() {}

ClientInfo::ClientInfo(const ClientInfo &other) {
	*this = other;
}

ClientInfo &ClientInfo::operator=(const ClientInfo &other) {
	if (this != &other) {
		fd_          = other.fd_;
		listen_ip_   = other.listen_ip_;
		listen_port_ = other.listen_port_;
	}
	return *this;
}

int ClientInfo::GetFd() const {
	return fd_;
}

const std::string &ClientInfo::GetListenIp() const {
	return listen_ip_;
}

unsigned int ClientInfo::GetListenPort() const {
	return listen_port_;
}

} // namespace server
