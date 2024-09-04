#include "virtual_server.hpp"

namespace server {

VirtualServer::VirtualServer() : client_max_body_size_(DEFAULT_CLIENT_MAX_BODY_SIZE) {}

VirtualServer::VirtualServer(
	const std::string  &server_name,
	const LocationList &locations,
	const HostPortList &host_ports,
	std::size_t         client_max_body_size
)
	: server_name_(server_name),
	  locations_(locations),
	  host_ports_(host_ports),
	  client_max_body_size_(client_max_body_size) {}

VirtualServer::~VirtualServer() {}

VirtualServer::VirtualServer(const VirtualServer &other) {
	*this = other;
}

VirtualServer &VirtualServer::operator=(const VirtualServer &other) {
	if (this != &other) {
		server_name_          = other.server_name_;
		locations_            = other.locations_;
		host_ports_           = other.host_ports_;
		client_max_body_size_ = other.client_max_body_size_;
	}
	return *this;
}

const std::string &VirtualServer::GetServerName() const {
	return server_name_;
}

// todo: not used yet
const VirtualServer::LocationList &VirtualServer::GetLocations() const {
	return locations_;
}

const VirtualServer::HostPortList &VirtualServer::GetHostPortList() const {
	return host_ports_;
}

std::size_t VirtualServer::GetClientMaxBodySize() const {
	return client_max_body_size_;
}


} // namespace server
