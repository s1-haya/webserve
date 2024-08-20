#include "virtual_server.hpp"

namespace server {

VirtualServer::VirtualServer() {}

VirtualServer::VirtualServer(
	const std::string &server_name, const LocationList &locations, const PortList &ports
)
	: server_name_(server_name), locations_(locations), ports_(ports) {}

VirtualServer::~VirtualServer() {}

VirtualServer::VirtualServer(const VirtualServer &other) {
	*this = other;
}

VirtualServer &VirtualServer::operator=(const VirtualServer &other) {
	if (this != &other) {
		server_name_ = other.server_name_;
		locations_   = other.locations_;
		ports_       = other.ports_;
	}
	return *this;
}

const std::string &VirtualServer::GetServerName() const {
	return server_name_;
}

const VirtualServer::LocationList &VirtualServer::GetLocations() const {
	return locations_;
}

const VirtualServer::PortList &VirtualServer::GetPorts() const {
	return ports_;
}

} // namespace server
