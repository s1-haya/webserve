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

void ContextManager::AddServerInfo(
	const ServerInfo &server_info, const VirtualServer *virtual_server
) {
	const int server_fd = server_info.GetFd();
	virtual_servers_.AddMapping(server_fd, virtual_server);
	if (sock_context_.IsServerInfoExist(server_fd)) {
		return;
	}
	sock_context_.AddServerInfo(server_fd, server_info);
}

void ContextManager::AddClientInfo(const ClientInfo &client_info, int server_fd) {
	const int client_fd = client_info.GetFd();
	sock_context_.AddClientInfo(client_fd, client_info, server_fd);
}

void ContextManager::DeleteClientInfo(int client_fd) {
	sock_context_.DeleteClientInfo(client_fd);
}

const VirtualServerStorage::VirtualServerList &ContextManager::GetAllVirtualServer() const {
	return virtual_servers_.GetAllVirtualServerList();
}

const VirtualServerStorage::VirtualServerAddrList &
ContextManager::GetVirtualServerAddrList(int client_fd) const {
	const ServerInfo &server_info = sock_context_.GetConnectedServerInfo(client_fd);
	const int         server_fd   = server_info.GetFd();

	return virtual_servers_.GetVirtualServerAddrList(server_fd);
}

ContextManager::GetServerInfoResult
ContextManager::GetServerInfo(const std::string &host, unsigned int port) const {
	return sock_context_.GetServerInfo(host, port);
}

} // namespace server
