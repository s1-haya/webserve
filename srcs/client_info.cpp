#include "client_info.hpp"

// todo: tmp
#include <iostream>

namespace server {

ClientInfo::ClientInfo(int fd, const struct sockaddr_storage &sock_addr) : fd_(fd), port_(0) {
	(void)sock_addr;
}

ClientInfo::~ClientInfo() {
	// todo: close?
	// if (fd_ != SYSTEM_ERROR) {
	// 	close(fd_);
	// }
}

} // namespace server
