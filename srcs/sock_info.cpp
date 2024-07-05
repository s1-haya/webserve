#include "sock_info.hpp"

namespace server {

int SockInfo::GetFd() const {
	return fd_;
}

struct sockaddr_in &SockInfo::GetSockAddr() {
	return sock_addr_;
}

socklen_t SockInfo::GetAddrlen() const {
	return addrlen_;
}

void SockInfo::SetSockFd(int fd) {
	fd_ = fd;
}

} // namespace server
