#ifndef HTTP_RESULT_HPP_
#define HTTP_RESULT_HPP_

#include <string>

namespace http {

struct HttpResult {
	HttpResult() : is_response_complete(false) {}

	bool        is_response_complete;
	std::string response;
};

} // namespace http

#endif /* HTTP_RESULT_HPP_ */
