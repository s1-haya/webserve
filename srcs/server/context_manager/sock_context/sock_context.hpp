#ifndef SERVER_CONTEXTMANAGER_SOCKCONTEXT_SOCKCONTEXT_HPP_
#define SERVER_CONTEXTMANAGER_SOCKCONTEXT_SOCKCONTEXT_HPP_

#include <map>

namespace server {

class ServerInfo;
class ClientInfo;

// Store socket informations for each fd
class SockContext {
  public:
	typedef std::map<int, ServerInfo>         ServerInfoMap;
	typedef std::map<int, ClientInfo>         ClientInfoMap;
	typedef std::map<int, const ServerInfo *> HostServerInfoMap;
	SockContext();
	~SockContext();
	SockContext(const SockContext &other);
	SockContext &operator=(const SockContext &other);
	// functions
	void AddServerInfo(int server_fd, const ServerInfo &server_info);
	bool IsServerInfoExist(int server_fd) const;
	void AddClientInfo(int client_fd, const ClientInfo &client_info, int server_fd);
	void DeleteClientInfo(int client_fd);
	// getter
	const ClientInfo &GetClientInfo(int client_fd) const;
	const ServerInfo &GetConnectedServerInfo(int client_fd) const;

  private:
	// function
	const ServerInfo *GetServerInfo(int server_fd) const;
	// const
	static const int SYSTEM_ERROR = -1;
	// variables
	ServerInfoMap     server_context_;
	ClientInfoMap     client_context_;
	HostServerInfoMap host_servers_;
};

} // namespace server

#endif /* SERVER_CONTEXTMANAGER_SOCKCONTEXT_SOCKCONTEXT_HPP_ */
