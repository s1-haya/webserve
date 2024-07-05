#include "sock_context.hpp"
#include "sock_info.hpp"
#include <unistd.h> // close

namespace server {

SockContext::SockContext() {}

SockContext::~SockContext() {
	typedef std::map<int, SockInfo>::iterator Itr;
	for (Itr it = context_.begin(); it != context_.end(); ++it) {
		const int fd = it->first;
		if (fd != SYSTEM_ERROR) {
			close(fd);
		}
	}
}

SockInfo &SockContext::GetSockInfo(int fd) {
	SockInfo &sock_info = context_.at(fd);
	// todo: fd doesn't exist, throw logic_error
	return sock_info;
}

} // namespace server
