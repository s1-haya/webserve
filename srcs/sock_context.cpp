#include "sock_context.hpp"
#include "sock_info.hpp"
#include <stdexcept> // logic_error
#include <unistd.h>  // close

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

void SockContext::AddSockInfo(int fd, const SockInfo &sock_info) {
	if (context_.count(fd) > 0) {
		throw std::logic_error("SockInfo already exists");
	}
	context_[fd] = sock_info;
}

void SockContext::DeleteSockInfo(int fd) {
	context_.erase(fd);
}

SockInfo &SockContext::GetSockInfo(int fd) {
	SockInfo &sock_info = context_.at(fd);
	if (context_.count(fd) == 0) {
		throw std::logic_error("SockInfo doesn't exist");
	}
	return sock_info;
}

} // namespace server
