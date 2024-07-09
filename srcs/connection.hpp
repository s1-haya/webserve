#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include <netdb.h> // struct addrinfo,gai_strerror
#include <set>
#include <string>

namespace server {

class SockInfo;

class Connection {
  public:
	typedef struct addrinfo AddrInfo;
	typedef std::set<int>   FdSet;
	Connection();
	~Connection();
	// function
	int        Connect(SockInfo &server_sock_info);
	static int Accept(SockInfo &sock_info);
	bool       IsListenServerFd(int sock_fd) const;

  private:
	// prohibit copy
	Connection(const Connection &other);
	Connection &operator=(const Connection &other);
	// function
	AddrInfo *GetAddrInfoList(const SockInfo &server_sock_info);
	// const
	static const int SYSTEM_ERROR = -1;
	// variable
	FdSet listen_server_fds_;
};

} // namespace server

#endif /* CONNECTION_HPP_ */
