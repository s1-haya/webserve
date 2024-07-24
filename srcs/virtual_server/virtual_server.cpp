#include "virtual_server.hpp"

namespace server {

VirtualServer::VirtualServer(const std::string &server_name, const std::string &location)
	: server_name_(server_name), location_(location) {
	// todo: error handling?
}

VirtualServer::~VirtualServer() {}

VirtualServer::VirtualServer(const VirtualServer &other) {
	*this = other;
}

VirtualServer &VirtualServer::operator=(const VirtualServer &other) {
	if (this != &other) {
		server_name_ = other.server_name_;
		location_    = other.location_;
	}
	return *this;
}

} // namespace server
