#include "virtual_server_storage.hpp"
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

// todo: update
// server_fdが新規なら作成して良いのでmap[]を使用
// e.g., VirtualServerAddrListMap -> {{127.0.0.1:8080  : List{vs1*, vs2*, vs3*}},
//                                    {0.0.0.0:8080    : List{vs2*}},
//                                    {172.19.0.1:8080 : List{vs2*, vs3*}}}
void VirtualServerStorage::AddMapping(
	const HostPortPair &host_port, const VirtualServer *virtual_server
) {
	VirtualServerAddrList &virtual_server_addr_list = virtual_server_addr_list_map_[host_port];
	virtual_server_addr_list.push_back(virtual_server);
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

} // namespace server
