#ifndef SERVER_DTO_HPP_
#define SERVER_DTO_HPP_

#include "virtual_server.hpp"
#include <string>

namespace server {

struct DtoServerInfos {
	int                         fd;
	std::string                 server_name;
	std::string                 port;
	VirtualServer::LocationList locations;
};

} // namespace server

#endif /* SERVER_DTO_HPP_ */