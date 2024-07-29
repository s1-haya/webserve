#include "virtual_server.hpp"

namespace server {

VirtualServer::VirtualServer() {}

VirtualServer::VirtualServer(const std::string &server_name, const LocationList &locations)
	: server_name_(server_name), locations_(locations) {
	// todo: error handling?
}

VirtualServer::~VirtualServer() {}

VirtualServer::VirtualServer(const VirtualServer &other) {
	*this = other;
}

VirtualServer &VirtualServer::operator=(const VirtualServer &other) {
	if (this != &other) {
		server_name_ = other.server_name_;
		locations_   = other.locations_;
	}
	return *this;
}

const std::string &VirtualServer::GetServerName() const {
	return server_name_;
}

const VirtualServer::LocationList &VirtualServer::GetLocations() const {
	return locations_;
}

} // namespace server
