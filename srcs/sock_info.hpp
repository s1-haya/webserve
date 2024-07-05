#ifndef SOCK_INFO_HPP_
#define SOCK_INFO_HPP_

#include <netinet/in.h> // struct sockaddr_in
#include <string>

namespace server {

class SockInfo {
  private:
	SockInfo();
	~SockInfo();
	// prohibit copy
	SockInfo(const SockInfo &other);
	SockInfo &operator=(const SockInfo &other);
	// variables
	int                fd_;
	std::string        name_;
	unsigned int       port_;
	struct sockaddr_in sock_addr_;
	socklen_t          addrlen_;
};

} // namespace server

#endif /* SOCK_INFO_HPP_ */
