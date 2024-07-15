#ifndef HTTP_PARSE_HPP_
#define HTTP_PARSE_HPP_

#include <map>
#include <string>
#include <vector>

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

struct HttpRequest {
	RequestLine  request_line;
	HeaderFields header_fields;
	std::string  message_body;
};

class HttpParse {
  public:
	static HttpRequest Run(const std::string &buf);

  private:
	HttpParse();
	~HttpParse();
	static RequestLine  SetRequestLine(const std::vector<std::string> &request_line);
	static HeaderFields SetHeaderFields(const std::vector<std::string> &header_fields_info);

	static std::string CheckMethod(const std::string &method);
	static std::string CheckRequestTarget(const std::string &request_target);
	static std::string CheckVersion(const std::string &version);
};

} // namespace http

#endif
