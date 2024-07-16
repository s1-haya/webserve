#ifndef CLIENT_INFO_HPP_
#define CLIENT_INFO_HPP_

#include <string>
#include <sys/socket.h> // struct sockaddr_storage,socklen_t

namespace server {

class ClientInfo {
  public:
	ClientInfo(int fd, const struct sockaddr_storage &sock_addr);
	~ClientInfo();

  private:
	ClientInfo();
	// prohibit copy
	ClientInfo(const ClientInfo &other);
	ClientInfo &operator=(const ClientInfo &other);
	// function
	void SetSockInfo(const struct sockaddr_storage &sock_addr);
	// variables
	int          fd_;
	std::string  ip_str_;
	unsigned int port_;
};

} // namespace server

#endif /* CLIENT_INFO_HPP_ */
