#ifndef HTTP_PARSE_HPP_
#define HTTP_PARSE_HPP_

#include <map>
#include <string>

namespace http {

struct RequestLine {
	std::string method;
	std::string uri;
	std::string version;
	RequestLine() : method(""), uri(""), version("") {}
	RequestLine(const std::string &method, const std::string &uri, const std::string &version)
		: method(method), uri(uri), version(version) {}
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
};

} // namespace http

#endif
