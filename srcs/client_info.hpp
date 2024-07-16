#ifndef CLIENT_INFO_HPP_
#define CLIENT_INFO_HPP_

#include <string>
#include <sys/socket.h> // struct sockaddr_storage,socklen_t

namespace server {

class ClientInfo {
  public:
	// default constructor: necessary for map's insert/[]
	ClientInfo();
	ClientInfo(int fd, const struct sockaddr_storage &sock_addr);
	~ClientInfo();
	ClientInfo(const ClientInfo &other);
	ClientInfo &operator=(const ClientInfo &other);
	// getter
	const std::string &GetIp() const;

  private:
	// function
	void SetSockInfo(const struct sockaddr_storage &sock_addr);
	// variables
	int         fd_;
	std::string ip_;
};

} // namespace server

#endif /* CLIENT_INFO_HPP_ */
