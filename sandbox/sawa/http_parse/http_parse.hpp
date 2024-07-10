#ifndef HTTP_PARSE_HPP_
#define HTTP_PARSE_HPP_

#include <map>
#include <string>

namespace http {

struct StatusLine {
	std::string method;
	std::string uri;
	std::string version;
};

typedef std::map<std::string, std::string> HeaderField;

struct HTTPRequest {
	StatusLine  status_line;
	HeaderField header_fields;
	std::string message_body;
};

class HTTPParse {
  public:
	HTTPParse();
	~HTTPParse();
	HTTPRequest Run(const std::string &buf);

  private:
	StatusLine SetStatusLine(const std::vector<std::string> &request_line);
};

} // namespace http

#endif
