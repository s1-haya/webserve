#include "cgi_manager.hpp"

namespace server {

CgiManager::CgiManager() {}

CgiManager::~CgiManager() {
	for (ItrCgiAddrMap it = cgi_addr_map_.begin(); it != cgi_addr_map_.end(); ++it) {
		delete it->second;
	}
}

// todo: 1complete_request(1CGI)につき2回以上この関数が呼ばれないかhttpの実装を確認
void CgiManager::AddNewCgi(int client_fd, const cgi::CgiRequest &request) {
	// todo: new error handling
	Cgi *cgi = new Cgi(request);
	// todo: add logic_error?
	cgi_addr_map_[client_fd] = cgi;
}

} // namespace server
