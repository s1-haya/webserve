#ifndef SERVER_DTO_HPP_
#define SERVER_DTO_HPP_

#include "virtual_server.hpp"
#include <string>

namespace server {

typedef std::list<const VirtualServer *> VirtualServerAddrList;

struct DtoClientInfos {
	int         fd;
	std::string request_buf;
};

} // namespace server

#endif /* SERVER_DTO_HPP_ */
