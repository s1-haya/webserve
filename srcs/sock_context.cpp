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

void SockContext::AddClientInfo(int fd, const ClientInfo &client_info) {
	typedef std::pair<ClientInfoMap::const_iterator, bool> InsertResult;
	InsertResult result = client_context_.insert(std::make_pair(fd, client_info));
	if (result.second == false) {
		throw std::logic_error("ClientInfo already exists");
	}
}

void SockContext::DeleteClientInfo(int fd) {
	client_context_.erase(fd);
}

} // namespace server
