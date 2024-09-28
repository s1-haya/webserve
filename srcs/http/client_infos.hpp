#ifndef CLIENT_INFOS_HPP_
#define CLIENT_INFOS_HPP_

#include <string>

namespace http {

struct ClientInfos {
	int          fd;
	std::string  ip;
	unsigned int listen_server_port;
	std::string  request_buf;
};

} // namespace http

#endif
