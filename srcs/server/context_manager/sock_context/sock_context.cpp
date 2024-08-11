#include "sock_context.hpp"
#include "client_info.hpp"
#include "server_info.hpp"
#include <stdexcept> // logic_error
#include <unistd.h>  // close

namespace server {

SockContext::SockContext() {}

SockContext::~SockContext() {
	typedef std::map<int, ServerInfo>::iterator Itr;
	for (Itr it = server_context_.begin(); it != server_context_.end(); ++it) {
		const int fd = it->first;
		if (fd != SYSTEM_ERROR) {
			close(fd);
		}
	}
}

SockContext::SockContext(const SockContext &other) {
	*this = other;
}

SockContext &SockContext::operator=(const SockContext &other) {
	if (this != &other) {
		server_context_ = other.server_context_;
		client_context_ = other.client_context_;
		host_servers_   = other.host_servers_;
	}
	return *this;
}

void SockContext::AddServerInfo(int server_fd, const ServerInfo &server_info) {
	typedef std::pair<ServerInfoMap::const_iterator, bool> InsertResult;
	InsertResult result = server_context_.insert(std::make_pair(server_fd, server_info));
	if (result.second == false) {
		throw std::logic_error("ServerInfo already exists");
	}
}

void SockContext::AddClientInfo(int client_fd, const ClientInfo &client_info, int server_fd) {
	// add client_info to client_context
	typedef std::pair<ClientInfoMap::const_iterator, bool> ResultInsertToClientContext;
	ResultInsertToClientContext                            result_client_context =
		client_context_.insert(std::make_pair(client_fd, client_info));
	if (result_client_context.second == false) {
		throw std::logic_error("ClientInfo already exists");
	}

	// add the connected host server_info associated with client_fd to host_servers
	typedef std::pair<HostServerInfoMap::const_iterator, bool> ResultInsertToHostServers;
	ResultInsertToHostServers                                  result_host_servers =
		host_servers_.insert(std::make_pair(client_fd, GetServerInfo(server_fd)));
	if (result_host_servers.second == false) {
		throw std::logic_error("ClientInfo already exists");
	}
}

void SockContext::DeleteClientInfo(int client_fd) {
	client_context_.erase(client_fd);
	host_servers_.erase(client_fd);
}

const ClientInfo &SockContext::GetClientInfo(int client_fd) const {
	try {
		return client_context_.at(client_fd);
	} catch (const std::exception &e) {
		throw std::logic_error("ClientInfo doesn't exist");
	}
}

const ServerInfo &SockContext::GetConnectedServerInfo(int client_fd) const {
	try {
		return *host_servers_.at(client_fd);
	} catch (const std::exception &e) {
		throw std::logic_error("associated ServerInfo doesn't exist");
	}
}

const ServerInfo *SockContext::GetServerInfo(int server_fd) const {
	try {
		return &server_context_.at(server_fd);
	} catch (const std::exception &e) {
		throw std::logic_error("ServerInfo doesn't exist");
	}
}

} // namespace server
