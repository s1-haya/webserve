#ifndef HTTP_PARSE_HPP_
#define HTTP_PARSE_HPP_

#include <map>
#include <string>

namespace http {

struct RequestLine {
	std::string method;
	std::string uri;
	std::string version;
};

typedef std::map<std::string, std::string> HeaderFields;

struct HTTPRequest {
	RequestLine  status_line;
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
};

} // namespace http

#endif
