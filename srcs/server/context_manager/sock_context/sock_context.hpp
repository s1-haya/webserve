#ifndef SERVER_CONTEXTMANAGER_SOCKCONTEXT_SOCKCONTEXT_HPP_
#define SERVER_CONTEXTMANAGER_SOCKCONTEXT_SOCKCONTEXT_HPP_

#include <map>

namespace server {

class ServerInfo;
class ClientInfo;

// Store socket informations for each fd
class SockContext {
  public:
	typedef std::map<int, ServerInfo> ServerInfoMap;
	typedef std::map<int, ClientInfo> ClientInfoMap;

	SockContext();
	~SockContext();
	SockContext(const SockContext &other);
	SockContext &operator=(const SockContext &other);
	// functions
	void AddServerInfo(int server_fd, const ServerInfo &server_info);
	void AddClientInfo(int client_fd, const ClientInfo &client_info);
	void DeleteClientInfo(int client_fd);
	// getter
	const ClientInfo &GetClientInfo(int client_fd) const;

  private:
	// const
	static const int SYSTEM_ERROR = -1;
	// variables
	ServerInfoMap server_context_;
	ClientInfoMap client_context_;
};

} // namespace server

#endif /* SERVER_CONTEXTMANAGER_SOCKCONTEXT_SOCKCONTEXT_HPP_ */
