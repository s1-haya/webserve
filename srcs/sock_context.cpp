#include "sock_context.hpp"
#include "client_info.hpp"
#include "sock_info.hpp"
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

void SockContext::DeleteServerInfo(int fd) {
	server_context_.erase(fd);
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

// In C++98, the map's "at" method is unavailable, not using const qualifiers.
ServerInfo &SockContext::GetServerInfo(int fd) {
	ServerInfo &server_info = server_context_[fd];
	if (server_context_.count(fd) == 0) {
		throw std::logic_error("ServerInfo doesn't exist");
	}
	return server_info;
}

} // namespace server
