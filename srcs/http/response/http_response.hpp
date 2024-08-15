#ifndef HTTP_RESPONSE_HPP_
#define HTTP_RESPONSE_HPP_

#include "status_code.hpp"
#include <map>
#include <string>

namespace http {

struct HttpRequestResult;

struct StatusLine {
	std::string version;
	std::string status_code;
	std::string reason_phrase;
};

typedef std::map<std::string, std::string> HeaderFields;

struct HttpResponseResult {
	StatusLine   status_line;
	HeaderFields header_fields;
	std::string  body_message;
};

class HttpResponse {
  public:
	static std::string CreateHttpResponse(const HttpResponseResult &response);
	// feature private
	static HttpResponseResult CreateHttpResponseResult(const HttpRequestResult &request_info);
	static HttpResponseResult CreateErrorHttpResponseResult(const HttpRequestResult &request_info);
	static void               GetHandler(const std::string &path, std::string &body_message);
	static void               PostHandler(
					  const std::string &path,
					  const std::string &request_body_message,
					  std::string       &response_body_message
				  );

  private:
	HttpResponse();
	~HttpResponse();

	static std::string
	CreateErrorBodyMessage(const std::string &status_code, const std::string &reason_phrase);
};

} // namespace http

#endif
