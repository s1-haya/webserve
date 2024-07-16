#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include <netdb.h> // struct addrinfo,gai_strerror
#include <set>
#include <string>

namespace server {

class SockInfo;
class ClientInfo;

class Connection {
  public:
	typedef struct addrinfo AddrInfo;
	typedef std::set<int>   FdSet;
	Connection();
	~Connection();
	// function
	int               Connect(SockInfo &server_sock_info);
	static ClientInfo Accept(int server_fd);
	bool              IsListenServerFd(int sock_fd) const;

  private:
	// prohibit copy
	Connection(const Connection &other);
	Connection &operator=(const Connection &other);
	// functions
	AddrInfo *GetAddrInfoList(const SockInfo &server_sock_info);
	int       TryBind(AddrInfo *addrinfo) const;
	// const
	static const int SYSTEM_ERROR = -1;
	// variable
	FdSet listen_server_fds_;
};

} // namespace server

#endif /* CONNECTION_HPP_ */
