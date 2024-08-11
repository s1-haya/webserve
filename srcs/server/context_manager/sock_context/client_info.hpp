#ifndef SERVER_CONTEXTMANAGER_SOCKCONTEXT_CLIENTINFO_HPP_
#define SERVER_CONTEXTMANAGER_SOCKCONTEXT_CLIENTINFO_HPP_

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
	int                GetFd() const;
	const std::string &GetIp() const;

  private:
	// function
	void SetSockInfo(const struct sockaddr_storage &sock_addr);
	// variables
	int         fd_;
	std::string ip_;
};

} // namespace server

#endif /* SERVER_CONTEXTMANAGER_SOCKCONTEXT_CLIENTINFO_HPP_ */
