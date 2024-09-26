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
	StatusLine() {};
	StatusLine(
		const std::string &version, const std::string &status_code, const std::string &reason_phrase
	)
		: version(version), status_code(status_code), reason_phrase(reason_phrase) {};
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
	HttpResponseFormat() {};
	HttpResponseFormat(
		const StatusLine   &status_line,
		const HeaderFields &header_fields,
		const std::string  &body_message
	)
		: status_line(status_line), header_fields(header_fields), body_message(body_message) {};
	StatusLine   status_line;
	HeaderFields header_fields;
	std::string  body_message;
};

} // namespace http

#endif
