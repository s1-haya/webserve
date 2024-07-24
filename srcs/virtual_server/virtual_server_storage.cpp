#include "virtual_server_storage.hpp"
#include "virtual_server.hpp"

namespace server {

VirtualServerStorage::VirtualServerStorage() {}

VirtualServerStorage::~VirtualServerStorage() {}

VirtualServerStorage::VirtualServerStorage(const VirtualServerStorage &other) {
	*this = other;
}

VirtualServerStorage &VirtualServerStorage::operator=(const VirtualServerStorage &other) {
	if (this != &other) {
		virtual_servers_ = other.virtual_servers_;
	}
	return *this;
}

} // namespace server
