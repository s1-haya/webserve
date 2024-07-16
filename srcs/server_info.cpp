#include "server_info.hpp"

namespace server {

ServerInfo::ServerInfo(int fd, const std::string &server_name, unsigned int port)
	: fd_(fd), server_name_(server_name), listen_port_(port) {}

ServerInfo::~ServerInfo() {
	// todo: close?
	// if (fd_ != SYSTEM_ERROR) {
	// 	close(fd_);
	// }
}

} // namespace server
