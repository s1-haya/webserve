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
	if (server_context_.count(fd) > 0) {
		throw std::logic_error("ServerInfo already exists");
	}
	server_context_[fd] = server_info;
}

void SockContext::AddClientInfo(int fd, const ClientInfo &client_info) {
	if (client_context_.count(fd) > 0) {
		throw std::logic_error("ClientInfo already exists");
	}
	client_context_[fd] = client_info;
}

void SockContext::DeleteClientInfo(int fd) {
	client_context_.erase(fd);
}

} // namespace server
