#ifndef HTTP_RESULT_HPP_
#define HTTP_RESULT_HPP_

#include "cgi_parse.hpp" // todo: CgiRequestをcgi.hppに移動して#include "cgi.hpp"にしたい
#include <string>

namespace http {

struct HttpResult {
	HttpResult() : is_response_complete(false), is_connection_keep(true), is_cgi(false) {}

	bool        is_response_complete;
	bool        is_connection_keep;
	std::string request_buf;
	std::string response;

	bool            is_cgi;
	cgi::CgiRequest cgi_request;
};

} // namespace http

#endif /* HTTP_RESULT_HPP_ */
