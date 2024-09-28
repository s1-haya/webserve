#include "context_manager.hpp"
#include "client_info.hpp"
#include "server_info.hpp"

namespace server {

ContextManager::ContextManager() {}

ContextManager::~ContextManager() {}

ContextManager::ContextManager(const ContextManager &other) {
	*this = other;
}

ContextManager &ContextManager::operator=(const ContextManager &other) {
	if (this != &other) {
		virtual_servers_ = other.virtual_servers_;
		sock_context_    = other.sock_context_;
	}
	return *this;
}

void ContextManager::AddVirtualServer(const VirtualServer &virtual_server) {
	virtual_servers_.AddVirtualServer(virtual_server);
}

void ContextManager::AddServerInfo(const HostPortPair &host_port) {
	sock_context_.AddServerInfo(host_port);
	virtual_servers_.InitHostPortPair(host_port);
}

void ContextManager::AddMapping(
	const HostPortPair &host_port, const VirtualServer *virtual_server
) {
	virtual_servers_.AddMapping(host_port, virtual_server);
}

void ContextManager::SetListenSockFd(const HostPortPair &host_port, int server_fd) {
	sock_context_.SetSockFd(host_port, server_fd);
}

void ContextManager::AddClientInfo(const ClientInfo &client_info) {
	const int client_fd = client_info.GetFd();
	sock_context_.AddClientInfo(client_fd, client_info);
}

void ContextManager::DeleteClientInfo(int client_fd) {
	sock_context_.DeleteClientInfo(client_fd);
}

const VirtualServerStorage::VirtualServerList &ContextManager::GetAllVirtualServer() const {
	return virtual_servers_.GetAllVirtualServerList();
}

const VirtualServerStorage::VirtualServerAddrList &
ContextManager::GetVirtualServerAddrList(int client_fd) const {
	const ClientInfo                 &client_info = sock_context_.GetClientInfo(client_fd);
	const VirtualServer::HostPortPair host_port =
		std::make_pair(client_info.GetListenIp(), client_info.GetListenPort());

	return virtual_servers_.GetVirtualServerAddrList(host_port);
}

const std::string &ContextManager::GetClientIp(int client_fd) const {
	const ClientInfo &client_info = sock_context_.GetClientInfo(client_fd);
	return client_info.GetIp();
}

} // namespace server
