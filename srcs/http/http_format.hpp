#ifndef HTTP_FORMAT_HPP_
#define HTTP_FORMAT_HPP_

#include <map>
#include <string>

namespace http {

struct RequestLine {
	std::string method;
	std::string request_target;
	std::string version;
};

struct StatusLine {
	std::string version;
	std::string status_code;
	std::string reason_phrase;
};

typedef std::map<std::string, std::string> HeaderFields;

struct HttpRequestFormat {
	RequestLine  request_line;
	HeaderFields header_fields;
	std::string  body_message;
};

struct HttpResponseFormat {
	StatusLine   status_line;
	HeaderFields header_fields;
	std::string  body_message;
};

} // namespace http

#endif
