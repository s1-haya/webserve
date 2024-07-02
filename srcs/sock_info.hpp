#ifndef SOCK_INFO_HPP_
#define SOCK_INFO_HPP_

#include <string>

namespace server {

struct SockInfo {
	// default constructor: necessary for map's operator[]
	SockInfo() : name(""), fd(0) {}
	SockInfo(const std::string &server_name, unsigned int fd) : name(server_name), fd(fd) {}
	SockInfo(const SockInfo &other) : name(other.name), fd(other.fd) {}
	SockInfo &operator=(const SockInfo &other) {
		if (this != &other) {
			name = other.name;
			fd   = other.fd;
		}
		return *this;
	}
	// variables
	std::string name;
	int         fd;
};

} // namespace server

#endif /* SOCK_INFO_HPP_ */
