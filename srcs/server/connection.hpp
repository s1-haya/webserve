#ifndef SERVER_CONNECTION_HPP_
#define SERVER_CONNECTION_HPP_

#include "result.hpp"
#include <list>
#include <netdb.h> // struct addrinfo,gai_strerror
#include <set>
#include <string>

namespace server {

class ServerInfo;
class ClientInfo;

class Connection {
  public:
	typedef struct addrinfo                      AddrInfo;
	typedef std::set<int>                        FdSet;
	typedef std::list<std::string>               IpList;
	typedef utils::Result<int>                   BindResult;
	typedef utils::Result<void>                  ListenResult;
	typedef std::pair<std::string, unsigned int> IpPortPair;
	typedef std::pair<std::string, unsigned int> HostPortPair;

	Connection();
	~Connection();
	// function
	static IpList     ResolveHostName(const std::string &hostname);
	int               Connect(const HostPortPair &host_port);
	static ClientInfo Accept(int server_fd);
	bool              IsListenServerFd(int sock_fd) const;

  private:
	// prohibit copy
	Connection(const Connection &other);
	Connection &operator=(const Connection &other);
	// functions
	AddrInfo         *GetAddrInfoList(const HostPortPair &host_port) const;
	BindResult        TryBind(AddrInfo *addrinfo) const;
	ListenResult      Listen(int server_fd);
	static IpPortPair GetListenIpPort(int client_fd);
	// const
	static const int SYSTEM_ERROR   = -1;
	static const int LISTEN_BACKLOG = 512;
	// variable
	FdSet listen_server_fds_;
};

} // namespace server

#endif /* SERVER_CONNECTION_HPP_ */
