#ifndef SERVER_DTO_HPP_
#define SERVER_DTO_HPP_

#include "virtual_server.hpp" // todo: remove
#include "virtual_server_storage.hpp"
#include <string>

namespace server {

struct DtoClientInfos {
	int         fd;
	std::string request_buf;
	std::string ip;
};

struct DtoServerInfos {
	int                         fd;
	std::string                 server_name; // todo: remove
	std::string                 port;        // todo: remove
	VirtualServer::LocationList locations;   // todo: remove

	VirtualServerStorage::VirtualServerAddrList virtual_server_addr_list;
};

} // namespace server

#endif /* SERVER_DTO_HPP_ */
