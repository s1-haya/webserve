#include "virtual_server.hpp"

namespace server {

VirtualServer::VirtualServer() {}

VirtualServer::VirtualServer(
	const std::string  &server_name,
	const LocationList &locations,
	const HostPortList &host_port_list
)
	: server_name_(server_name), locations_(locations), host_port_list_(host_port_list) {}

VirtualServer::~VirtualServer() {}

VirtualServer::VirtualServer(const VirtualServer &other) {
	*this = other;
}

VirtualServer &VirtualServer::operator=(const VirtualServer &other) {
	if (this != &other) {
		server_name_    = other.server_name_;
		locations_      = other.locations_;
		host_port_list_ = other.host_port_list_;
	}
	return *this;
}

const std::string &VirtualServer::GetServerName() const {
	return server_name_;
}

const VirtualServer::LocationList &VirtualServer::GetLocations() const {
	return locations_;
}

const VirtualServer::HostPortList &VirtualServer::GetHostPortList() const {
	return host_port_list_;
}

} // namespace server
