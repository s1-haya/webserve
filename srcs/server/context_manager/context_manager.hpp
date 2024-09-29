#ifndef SERVER_CONTEXTMANAGER_CONTEXTMANAGER_HPP_
#define SERVER_CONTEXTMANAGER_CONTEXTMANAGER_HPP_

#include "sock_context.hpp"
#include "virtual_server.hpp"
#include "virtual_server_storage.hpp"

namespace server {

// holds and manages virtual server info and socket context(server socket info, client socket info).
class ContextManager {
  public:
	typedef VirtualServer::HostPortPair HostPortPair;

	ContextManager();
	~ContextManager();
	ContextManager(const ContextManager &other);
	ContextManager &operator=(const ContextManager &other);
	// functions
	void AddVirtualServer(const VirtualServer &virtual_server);
	void AddMapping(const HostPortPair &host_port, const VirtualServer *virtual_server);
	void SetListenSockFd(const HostPortPair &host_port, int server_fd);
	void AddServerInfo(const HostPortPair &host_port);
	void AddClientInfo(const ClientInfo &client_info);
	void DeleteClientInfo(int client_fd);
	// getter
	const VirtualServerStorage::VirtualServerList     &GetAllVirtualServer() const;
	const VirtualServerStorage::VirtualServerAddrList &GetVirtualServerAddrList(int client_fd
	) const;
	const std::string                                 &GetClientIp(int client_fd) const;
	unsigned int                                       GetListenServerPort(int client_fd) const;

  private:
	VirtualServerStorage virtual_servers_;
	SockContext          sock_context_;
};

} // namespace server

#endif /* SERVER_CONTEXTMANAGER_CONTEXTMANAGER_HPP_ */
