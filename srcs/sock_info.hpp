#ifndef SOCK_INFO_HPP_
#define SOCK_INFO_HPP_

#include <netinet/in.h> // struct sockaddr_in
#include <string>

namespace server {

class SockInfo {
  public:
	// default constructor: necessary for map's insert/[]
	SockInfo();
	SockInfo(const std::string &name, unsigned int port);
	~SockInfo();
	SockInfo(const SockInfo &other);
	SockInfo &operator=(const SockInfo &other);
	// getter
	int                 GetFd() const;
	struct sockaddr_in &GetSockAddr();
	socklen_t           GetAddrlen() const;
	// setter
	void SetSockFd(int fd);
	void SetPeerSockFd(int fd);

  private:
	int          fd_;
	std::string  name_;
	unsigned int port_;
	// todo: use struct sockaddr
	struct sockaddr_in sock_addr_;
	socklen_t          addrlen_;
};

} // namespace server

#endif /* SOCK_INFO_HPP_ */
