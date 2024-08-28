#ifndef SERVER_DTO_HPP_
#define SERVER_DTO_HPP_

#include "virtual_server.hpp"
#include <string>

namespace server {

struct DtoClientInfos {
	int         fd;
	std::string request_buf;
	std::string ip;
};

struct DtoServerInfos {
	int                         fd;
	std::string                 server_name;
	std::string                 port;
	VirtualServer::LocationList locations;
};

} // namespace server

#endif /* SERVER_DTO_HPP_ */