#include "virtual_server.hpp"

namespace server {

VirtualServer::VirtualServer()
	: client_max_body_size_(DEFAULT_CLIENT_MAX_BODY_SIZE), error_page_(std::make_pair(0, "")) {}

VirtualServer::VirtualServer(
	const ServerNameList &server_names,
	const LocationList   &locations,
	const HostPortList   &host_ports,
	std::size_t           client_max_body_size,
	const ErrorPage      &error_page
)
	: server_names_(server_names),
	  locations_(locations),
	  host_ports_(host_ports),
	  client_max_body_size_(client_max_body_size),
	  error_page_(error_page) {}

VirtualServer::~VirtualServer() {}

VirtualServer::VirtualServer(const VirtualServer &other) {
	*this = other;
}

VirtualServer &VirtualServer::operator=(const VirtualServer &other) {
	if (this != &other) {
		server_names_         = other.server_names_;
		locations_            = other.locations_;
		host_ports_           = other.host_ports_;
		client_max_body_size_ = other.client_max_body_size_;
		error_page_           = other.error_page_;
	}
	return *this;
}

const VirtualServer::ServerNameList &VirtualServer::GetServerNameList() const {
	return server_names_;
}

// todo: not used yet
const VirtualServer::LocationList &VirtualServer::GetLocationList() const {
	return locations_;
}

const VirtualServer::HostPortList &VirtualServer::GetHostPortList() const {
	return host_ports_;
}

std::size_t VirtualServer::GetClientMaxBodySize() const {
	return client_max_body_size_;
}

const VirtualServer::ErrorPage &VirtualServer::GetErrorPage() const {
	return error_page_;
}

} // namespace server
