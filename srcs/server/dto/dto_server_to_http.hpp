#ifndef SERVER_DTO_HPP_
#define SERVER_DTO_HPP_

#include "virtual_server_storage.hpp"
#include <string>

namespace server {

struct DtoClientInfos {
	int         fd;
	std::string request_buf;
};

struct DtoServerInfos {
	int                                         fd;
	VirtualServerStorage::VirtualServerAddrList virtual_server_addr_list;
};

} // namespace server

#endif /* SERVER_DTO_HPP_ */
