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

void SockContext::AddServerInfo(int fd, const ServerInfo &server_info) {
	typedef std::pair<ServerInfoMap::const_iterator, bool> InsertResult;
	InsertResult result = server_context_.insert(std::make_pair(fd, server_info));
	if (result.second == false) {
		throw std::logic_error("ServerInfo already exists");
	}
}

void SockContext::AddClientInfo(int client_fd, const ClientInfo &client_info, int server_fd) {
	// add client_info to client_context
	typedef std::pair<ClientInfoMap::const_iterator, bool> ResultInsertToContext;
	ResultInsertToContext                                  result_context =
		client_context_.insert(std::make_pair(client_fd, client_info));
	if (result_context.second == false) {
		throw std::logic_error("ClientInfo already exists");
	}

	// add the connected host server_info associated with client_fd to host_servers
	typedef std::pair<ClientsHostServerMap::const_iterator, bool> ResultInsertToHost;
	ResultInsertToHost                                            result_host =
		host_servers_.insert(std::make_pair(client_fd, GetServerInfo(server_fd)));
	if (result_host.second == false) {
		throw std::logic_error("ClientInfo already exists");
	}
}

void SockContext::DeleteClientInfo(int fd) {
	client_context_.erase(fd);
	host_servers_.erase(fd);
}

const ServerInfo &SockContext::GetServerInfo(int fd) const {
	try {
		return server_context_.at(fd);
	} catch (const std::exception &e) {
		throw std::logic_error("ServerInfo doesn't exist");
	}
}

} // namespace server
