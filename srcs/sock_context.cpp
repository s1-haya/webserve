#include "sock_context.hpp"
#include "sock_info.hpp"

namespace server {

SockInfo &SockContext::GetSockInfo(int fd) {
	SockInfo &sock_info = context_.at(fd);
	// todo: fd doesn't exist, throw logic_error
	return sock_info;
}

} // namespace server
