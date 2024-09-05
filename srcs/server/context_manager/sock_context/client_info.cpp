#include "client_info.hpp"

namespace server {

ClientInfo::ClientInfo() : fd_(0) {}

ClientInfo::ClientInfo(int fd) : fd_(fd) {}

ClientInfo::~ClientInfo() {}

ClientInfo::ClientInfo(const ClientInfo &other) {
	*this = other;
}

ClientInfo &ClientInfo::operator=(const ClientInfo &other) {
	if (this != &other) {
		fd_ = other.fd_;
	}
	return *this;
}

int ClientInfo::GetFd() const {
	return fd_;
}

} // namespace server
