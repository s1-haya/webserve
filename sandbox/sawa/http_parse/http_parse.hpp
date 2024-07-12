#ifndef HTTP_PARSE_HPP_
#define HTTP_PARSE_HPP_

#include <map>
#include <string>
#include <vector>

namespace http {

struct RequestLine {
	std::string method;
	std::string uri;
	std::string version;
};

typedef std::map<std::string, std::string> HeaderFields;

struct HttpRequest {
	RequestLine  status_line;
	HeaderFields header_fields;
	std::string  message_body;
};

class HttpParse {
  public:
	HttpParse();
	~HttpParse();
	static HttpRequest Run(const std::string &buf);

  private:
	static RequestLine  SetRequestLine(const std::vector<std::string> &request_line);
	static HeaderFields SetHeaderFields(const std::vector<std::string> &header_fields_info);
	static std::string  SetMessageBody(const std::vector<std::string> &message_body_info);
};

} // namespace http

#endif
