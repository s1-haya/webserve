#include "virtual_server_storage.hpp"
#include "define.hpp"
#include <stdexcept> // logic_error

namespace server {

VirtualServerStorage::VirtualServerStorage() {}

VirtualServerStorage::~VirtualServerStorage() {}

VirtualServerStorage::VirtualServerStorage(const VirtualServerStorage &other) {
	*this = other;
}

VirtualServerStorage &VirtualServerStorage::operator=(const VirtualServerStorage &other) {
	if (this != &other) {
		virtual_servers_              = other.virtual_servers_;
		virtual_server_addr_list_map_ = other.virtual_server_addr_list_map_;
	}
	return *this;
}

void VirtualServerStorage::AddVirtualServer(const VirtualServer &virtual_server) {
	virtual_servers_.push_back(virtual_server);
}

// For 0.0.0.0, initialize all host_port_pair keys here before mapping virtual_server* to each.
void VirtualServerStorage::InitHostPortPair(const VirtualServer::HostPortPair &host_port) {
	virtual_server_addr_list_map_[host_port] = VirtualServerAddrList();
}

// e.g.,
// server {
// 	  listen 127.0.0.1:8080;
// }
// server {
// 	  listen 0.0.0.0:8080;
// }
// server {
// 	  listen 127.0.0.1:8080;
// 	  listen 172.19.0.1:8080;
// }
// VirtualServerAddrListMap -> {{127.0.0.1:8080  : List{vs1*, vs2*, vs3*}},
//                              {0.0.0.0:8080    : List{vs2*}},
//                              {172.19.0.1:8080 : List{vs2*, vs3*}}}
void VirtualServerStorage::AddMapping(
	const HostPortPair &host_port, const VirtualServer *virtual_server
) {
	if (host_port.first != IPV4_ADDR_ANY) {
		// Add to the specific host:port mapping
		VirtualServerAddrList &virtual_server_addr_list = virtual_server_addr_list_map_[host_port];
		AddVirtualServerAddr(virtual_server_addr_list, virtual_server);
		return;
	}
	// For "0.0.0.0", add the virtual_server* to all host:port pairs with the same port
	typedef VirtualServerAddrListMap::iterator Itr;
	for (Itr it = virtual_server_addr_list_map_.begin(); it != virtual_server_addr_list_map_.end();
		 ++it) {
		const unsigned int exist_port = it->first.second;
		if (exist_port == host_port.second) {
			VirtualServerAddrList &virtual_server_addr_list = it->second;
			AddVirtualServerAddr(virtual_server_addr_list, virtual_server);
		}
	}
}

// todo: update
const VirtualServerStorage::VirtualServerAddrList &
VirtualServerStorage::GetVirtualServerAddrList(const HostPortPair &host_port) const {
	try {
		return virtual_server_addr_list_map_.at(host_port);
	} catch (const std::exception &e) {
		throw std::logic_error("VirtualServerAddrList doesn't exists");
	}
}

const VirtualServerStorage::VirtualServerList &
VirtualServerStorage::GetAllVirtualServerList() const {
	return virtual_servers_;
}

void VirtualServerStorage::AddVirtualServerAddr(
	VirtualServerAddrList &virtual_server_addr_list, const VirtualServer *virtual_server
) {
	if (virtual_server_addr_list.size() > 0 && virtual_server_addr_list.back() == virtual_server) {
		// The virtual_server* is already added.
		return;
	}
	virtual_server_addr_list.push_back(virtual_server);
}

} // namespace server
