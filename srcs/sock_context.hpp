#ifndef SOCK_CONTEXT_HPP_
#define SOCK_CONTEXT_HPP_

#include <map>

namespace server {

class ServerInfo;
class ClientInfo;

// Store socket informations for each fd
class SockContext {
  public:
	typedef std::map<int, ServerInfo>         ServerInfoMap;
	typedef std::map<int, ClientInfo>         ClientInfoMap;
	typedef std::map<int, const ServerInfo *> ClientsHostServerMap; // todo: naming
	SockContext();
	~SockContext();
	// functions
	// todo: overload?
	void AddServerInfo(int fd, const ServerInfo &server_info);
	void AddClientInfo(int fd, const ClientInfo &client_info, int server_fd);
	void DeleteClientInfo(int fd);
	// getter
	const ServerInfo *GetServerInfo(int fd) const;
	const ClientInfo &GetClientInfo(int fd) const;
	const ServerInfo &GetConnectedServerInfo(int client_fd) const;

  private:
	// prohibit copy
	SockContext(const SockContext &other);
	SockContext &operator=(const SockContext &other);
	// const
	static const int SYSTEM_ERROR = -1;
	// variables
	ServerInfoMap        server_context_;
	ClientInfoMap        client_context_;
	ClientsHostServerMap host_servers_;
};

} // namespace server

#endif /* SOCK_CONTEXT_HPP_ */
