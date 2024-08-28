#include "context_manager.hpp"
#include "client_info.hpp"
#include "server_info.hpp"
#include "utils.hpp"

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
	sock_context_.AddServerInfo(server_fd, server_info);
}

void ContextManager::AddClientInfo(const ClientInfo &client_info, int server_fd) {
	const int client_fd = client_info.GetFd();
	sock_context_.AddClientInfo(client_fd, client_info, server_fd);
}

void ContextManager::DeleteClientInfo(int client_fd) {
	sock_context_.DeleteClientInfo(client_fd);
}

const VirtualServerStorage::VirtualServerList &ContextManager::GetVirtualServerList() const {
	return virtual_servers_.GetAllVirtualServerList();
}

ServerContext ContextManager::GetServerContext(int client_fd) const {
	// from sock_context
	const ServerInfo &server_info = sock_context_.GetConnectedServerInfo(client_fd);
	const int         server_fd   = server_info.GetFd();

	// from virtual_servers
	const VirtualServer &virtual_server = virtual_servers_.GetVirtualServer(server_fd);

	// create ServerContext
	ServerContext server_infos;
	server_infos.fd          = server_fd;
	server_infos.server_name = virtual_server.GetServerName();
	// todo: uintのままで良いかも？
	server_infos.port      = utils::ConvertUintToStr(server_info.GetPort());
	server_infos.locations = virtual_server.GetLocations();
	return server_infos;
}

// todo: IP以外も必要ならClientContext作って詰めて返す
const std::string &ContextManager::GetClientIp(int client_fd) const {
	const ClientInfo &client_info = sock_context_.GetClientInfo(client_fd);
	return client_info.GetIp();
}

} // namespace server
