#ifndef SOCK_INFO_HPP_
#define SOCK_INFO_HPP_

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
	int          GetFd() const;
	std::string  GetName() const;
	unsigned int GetPort() const;
	// setter
	void SetSockFd(int fd);
	void SetPeerSockFd(int fd);

  private:
	int          fd_;
	std::string  name_;
	unsigned int port_;
	// peer socket info(client)
	int peer_fd_;
};

} // namespace server

#endif /* SOCK_INFO_HPP_ */
