#ifndef CLIENT_INFOS_HPP_
#define CLIENT_INFOS_HPP_

#include <string>

namespace http {

struct MockDtoClientInfos {
	int         fd;
	std::string request_buf;
	std::string ip;
};

} // namespace http

#endif
