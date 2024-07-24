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
		virtual_servers_               = other.virtual_servers_;
		mapping_fd_to_virtual_servers_ = other.mapping_fd_to_virtual_servers_;
	}
	return *this;
}

void VirtualServerStorage::AddVirtualServer(const VirtualServer &virtual_server) {
	virtual_servers_.push_back(virtual_server);
}

// ex)
// - virtual_server1が通信でfd 4,5を使用している場合
// virtual_server_storage.AddMapping(4, virtual_server1*);
// virtual_server_storage.AddMapping(5, virtual_server1*);
// -> mapping_fd_to_virtual_servers_ = {{4, virtual_server1*}, {5, virtual_servers1*}}
void VirtualServerStorage::AddMapping(int server_fd, const VirtualServer *virtual_server) {
	typedef std::pair<VirtualServerMapping::const_iterator, bool> InsertResult;
	InsertResult                                                  result =
		mapping_fd_to_virtual_servers_.insert(std::make_pair(server_fd, virtual_server));
	if (result.second == false) {
		throw std::logic_error("server_fd is already mapped");
	}
}

} // namespace server
