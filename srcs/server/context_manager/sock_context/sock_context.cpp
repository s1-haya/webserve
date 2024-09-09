#include "sock_context.hpp"
#include "client_info.hpp"
#include "server_info.hpp"
#include <stdexcept> // logic_error

namespace server {

SockContext::SockContext() {}

SockContext::~SockContext() {}

SockContext::SockContext(const SockContext &other) {
	*this = other;
}

SockContext &SockContext::operator=(const SockContext &other) {
	if (this != &other) {
		server_context_ = other.server_context_;
		client_context_ = other.client_context_;
	}
	return *this;
}

// todo: server_context_無駄なことしてる
void SockContext::AddServerInfo(const HostPortPair &host_port) {
	if (server_context_.count(host_port) > 0) {
		return;
	}
	const ServerInfo server_info(host_port);
	server_context_[host_port] = server_info;
}

void SockContext::SetSockFd(const HostPortPair &host_port, int server_fd) {
	try {
		ServerInfo &server_info = server_context_.at(host_port);
		server_info.SetSockFd(server_fd);
	} catch (const std::exception &e) {
		throw std::logic_error("ServerInfo doesn't exist");
	}
}

void SockContext::AddClientInfo(int client_fd, const ClientInfo &client_info) {
	typedef std::pair<ClientInfoMap::const_iterator, bool> InsertResult;
	const InsertResult result = client_context_.insert(std::make_pair(client_fd, client_info));
	if (result.second == false) {
		throw std::logic_error("ClientInfo already exists");
	}
}

void SockContext::DeleteClientInfo(int client_fd) {
	client_context_.erase(client_fd);
}

const ClientInfo &SockContext::GetClientInfo(int client_fd) const {
	try {
		return client_context_.at(client_fd);
	} catch (const std::exception &e) {
		throw std::logic_error("ClientInfo doesn't exist");
	}
}

} // namespace server
