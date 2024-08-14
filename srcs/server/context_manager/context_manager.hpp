#ifndef SERVER_CONTEXTMANAGER_CONTEXTMANAGER_HPP_
#define SERVER_CONTEXTMANAGER_CONTEXTMANAGER_HPP_

#include "sock_context.hpp"
#include "virtual_server.hpp"
#include "virtual_server_storage.hpp"
#include <string>

namespace server {

struct ServerContext {
	int                         fd;
	std::string                 server_name;
	std::string                 port;
	VirtualServer::LocationList locations;
};

// holds and manages virtual server info and socket context(server socket info, client socket info).
class ContextManager {
  public:
	ContextManager();
	~ContextManager();
	ContextManager(const ContextManager &other);
	ContextManager &operator=(const ContextManager &other);
	// functions
	void AddVirtualServer(const VirtualServer &virtual_server);
	void AddServerInfo(const ServerInfo &server_info, const VirtualServer *virtual_server);
	void AddClientInfo(const ClientInfo &client_info, int server_fd);
	void DeleteClientInfo(int client_fd);
	// getter
	const VirtualServerStorage::VirtualServerList &GetVirtualServerList() const;
	ServerContext                                  GetServerContext(int client_fd) const;
	const std::string                             &GetClientIp(int client_fd) const;

  private:
	VirtualServerStorage virtual_servers_;
	SockContext          sock_context_;
};

} // namespace server

#endif /* SERVER_CONTEXTMANAGER_CONTEXTMANAGER_HPP_ */