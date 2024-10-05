#ifndef HTTP_RESULT_HPP_
#define HTTP_RESULT_HPP_

#include "cgi_request.hpp"
#include <string>

namespace http {

struct CgiResult {
	CgiResult() : is_cgi(false) {}

	bool            is_cgi;
	cgi::CgiRequest cgi_request;
};

struct HttpResult {
	HttpResult()
		: is_response_complete(false), is_connection_keep(true), is_local_redirect(false) {}

	bool        is_response_complete;
	bool        is_connection_keep;
	bool        is_local_redirect;
	std::string request_buf;
	std::string response;
	CgiResult   cgi_result;
};

} // namespace http

#endif /* HTTP_RESULT_HPP_ */
