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

// todo: update
void SockContext::AddServerInfo(int server_fd, const ServerInfo &server_info) {
	typedef std::pair<ServerInfoMap::const_iterator, bool> InsertResult;
	InsertResult result = server_context_.insert(std::make_pair(server_fd, server_info));
	if (result.second == false) {
		throw std::logic_error("ServerInfo already exists");
	}
}

// todo: update
void SockContext::AddClientInfo(int client_fd, const ClientInfo &client_info) {
	typedef std::pair<ClientInfoMap::const_iterator, bool> ResultInsertToClientContext;
	ResultInsertToClientContext                            result_client_context =
		client_context_.insert(std::make_pair(client_fd, client_info));
	if (result_client_context.second == false) {
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
