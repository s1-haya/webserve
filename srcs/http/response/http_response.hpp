#ifndef HTTP_RESPONSE_HPP_
#define HTTP_RESPONSE_HPP_

#include "http_parse.hpp"
#include <string>

namespace http {

struct StatusLine {
	std::string version;
	std::string status_code;
	std::string status_reason;
};

struct HttpResponseResult {
	StatusLine   status_line;
	HeaderFields header_fields;
	std::string  body_message;
};

class HttpResponse {
  public:
	static std::string CreateHttpResponse(const HttpResponseResult &response);

  private:
	HttpResponse();
	~HttpResponse();
	static void CreateStatusLine(std::ostream &response_stream, const StatusLine &status_line);
	static void CreateHeaderField(
		std::ostream &response_stream, const std::string &name, const std::string &value
	);
	static void
	CreateHeaderFields(std::ostream &response_stream, const HeaderFields &header_fields);
	static void CreateBodyMessage(std::ostream &response_stream, const std::string &body_message);
};

} // namespace http

#endif
