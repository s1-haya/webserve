#include "virtual_server_storage.hpp"
#include "virtual_server.hpp"
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

// server_fdが新規なら作成して良いのでmap[]を使用
// e.g., VirtualServerAddrListMap -> {{fd 4 : List{virtual_server1*, virtual_server2*}},
//                                    {fd 5 : List{virtual_server3*}},
//                                    {fd 6 : List{virtual_server2*, virtual_server3*}}}
void VirtualServerStorage::AddMapping(int server_fd, const VirtualServer *virtual_server) {
	VirtualServerAddrList &virtual_server_addr_list = virtual_server_addr_list_map_[server_fd];
	virtual_server_addr_list.push_back(virtual_server);
}

const VirtualServerStorage::VirtualServerAddrList &
VirtualServerStorage::GetVirtualServerAddrList(int server_fd) const {
	try {
		return virtual_server_addr_list_map_.at(server_fd);
	} catch (const std::exception &e) {
		throw std::logic_error("VirtualServerAddrList doesn't exists");
	}
}

// todo: tmp. need?
const VirtualServerStorage::VirtualServerList &
VirtualServerStorage::GetAllVirtualServerList() const {
	return virtual_servers_;
}

} // namespace server
