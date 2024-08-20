#ifndef HTTP_RESULT_HPP_
#define HTTP_RESULT_HPP_

#include <string>

namespace http {

struct HttpResult {
	HttpResult() : is_response_complete(false), is_connection_keep(true) {}

	bool        is_response_complete;
	bool        is_connection_keep;
	std::string request_buf;
	std::string response;
};

} // namespace http

#endif /* HTTP_RESULT_HPP_ */
