#ifndef HTTP_RESPONSE_HPP_
#define HTTP_RESPONSE_HPP_

#include <map>
#include <string>
#include "status_code.hpp"

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
	typedef std::map<StatusCode, std::string> ReasonPhrase;
	static std::string        CreateHttpResponse(const HttpResponseResult &response);
	static HttpResponseResult CreateHttpResponseResult(const HttpRequestResult &request_info);
	static HttpResponseResult CreateErrorHttpResponseResult(const HttpRequestResult &request_info);

  private:
	HttpResponse();
	~HttpResponse();

	static std::string
	CreateErrorBodyMessage(const std::string &status_code, const std::string &reason_phrase);
};

} // namespace http

#endif
