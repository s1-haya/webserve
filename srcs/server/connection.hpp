#ifndef SERVER_CONNECTION_HPP_
#define SERVER_CONNECTION_HPP_

#include "result.hpp"
#include <netdb.h> // struct addrinfo,gai_strerror
#include <set>
#include <string>

namespace server {

class ServerInfo;
class ClientInfo;

class Connection {
  public:
	typedef struct addrinfo AddrInfo;
	typedef std::set<int>   FdSet;
	typedef Result<int>     BindResult;
	Connection();
	~Connection();
	// function
	int               Connect(ServerInfo &server_info);
	static ClientInfo Accept(int server_fd);
	bool              IsListenServerFd(int sock_fd) const;

  private:
	// prohibit copy
	Connection(const Connection &other);
	Connection &operator=(const Connection &other);
	// functions
	AddrInfo  *GetAddrInfoList(const ServerInfo &server_info);
	BindResult TryBind(AddrInfo *addrinfo) const;
	// const
	static const int SYSTEM_ERROR = -1;
	// variable
	FdSet listen_server_fds_;
};

} // namespace server

#endif /* SERVER_CONNECTION_HPP_ */
