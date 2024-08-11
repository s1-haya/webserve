#ifndef SERVER_CONNECTION_HPP_
#define SERVER_CONNECTION_HPP_

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
	AddrInfo *GetAddrInfoList(const ServerInfo &server_info);
	int       TryBind(AddrInfo *addrinfo) const;
	// const
	static const int SYSTEM_ERROR = -1;
	// variable
	FdSet listen_server_fds_;
};

} // namespace server

#endif /* SERVER_CONNECTION_HPP_ */
