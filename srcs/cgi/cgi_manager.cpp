#include "cgi_manager.hpp"

namespace cgi {

CgiManager::CgiManager() {}

CgiManager::~CgiManager() {
	for (ItrCgiAddrMap it = client_cgi_map_.begin(); it != client_cgi_map_.end(); ++it) {
		delete it->second;
	}
}


} // namespace cgi
