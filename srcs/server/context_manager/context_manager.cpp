#include "context_manager.hpp"

namespace server {

ContextManager::ContextManager() {}

ContextManager::~ContextManager() {}

ContextManager::ContextManager(const ContextManager &other) {
	*this = other;
}

ContextManager &ContextManager::operator=(const ContextManager &other) {
	if (this != &other) {
		sock_context_ = other.sock_context_;
	}
	return *this;
}

void ContextManager::AddServerInfo(
	int server_fd, const ServerInfo &server_info, const VirtualServer *virtual_server
) {
	(void)virtual_server;
	sock_context_.AddServerInfo(server_fd, server_info);
}

void ContextManager::AddClientInfo(int client_fd, const ClientInfo &client_info, int server_fd) {
	sock_context_.AddClientInfo(client_fd, client_info, server_fd);
}

void ContextManager::DeleteClientInfo(int client_fd) {
	sock_context_.DeleteClientInfo(client_fd);
}

const ServerInfo &ContextManager::GetServerInfo(int client_fd) const {
	const ServerInfo &server_info = sock_context_.GetConnectedServerInfo(client_fd);
	return server_info;
}

// todo: IP以外も必要ならDTO作って詰めて返す
const std::string &ContextManager::GetClientInfo(int client_fd) const {
	const ClientInfo &client_info = sock_context_.GetClientInfo(client_fd);
	return client_info.GetIp();
}

} // namespace server
