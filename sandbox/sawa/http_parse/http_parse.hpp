#ifndef HTTP_PARSE_HPP_
#define HTTP_PARSE_HPP_

#include <map>
#include <string>

namespace http {

struct RequestLine {
	std::string method;
	std::string request_target;
	std::string version;
	RequestLine() : method(""), request_target(""), version("") {}
	RequestLine(
		const std::string &method, const std::string &request_target, const std::string &version
	)
		: method(method), request_target(request_target), version(version) {}
};

typedef std::map<std::string, std::string> HeaderFields;

struct HTTPRequest {
	RequestLine  request_line;
	HeaderFields header_fields;
	std::string  message_body;
};

class HTTPParse {
  public:
	HTTPParse();
	~HTTPParse();
	HTTPRequest Run(const std::string &buf);

  private:
	RequestLine  SetRequestLine(const std::vector<std::string> &request_line);
	HeaderFields SetHeaderFields(const std::vector<std::string> &header_fields_info);
	std::string  SetMessageBody(const std::vector<std::string> &message_body_info);

	std::string CheckMethod(const std::string &method);
	std::string CheckRequestTarget(const std::string &request_target);
	std::string CheckVersion(const std::string &version);
};

} // namespace http

#endif
