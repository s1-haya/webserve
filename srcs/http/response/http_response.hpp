#ifndef HTTP_RESPONSE_HPP_
#define HTTP_RESPONSE_HPP_

#include "http_parse.hpp"
#include <map>
#include <string>

namespace http {

struct StatusLine {
	std::string version;
	std::string status_code;
	std::string reason_phrase;
};

struct HttpResponseResult {
	StatusLine   status_line;
	HeaderFields header_fields;
	std::string  body_message;
};

// HttpResponse {
// public:
//   static std::string Run(HttpRequestResult)
//   static std::string CreateTimeoutRequestResponse(HttpRequestResult)
//   static std::string CreateInternalServerErrorResponse(HttpRequestResult)
//  private:
//   static std::string        CreateHttpResponseFormat(const HttpResponseResult &response);
//   static HttpResponseResult CreateHttpResponseResult(const HttpRequestResult &request_info);
//   static HttpResponseResult CreateSuccessHttpResponseResult(const HttpRequestResult
//   &request_info);
//   static HttpResponseResult CreateErrorHttpResponseResult(const HttpRequestResult
//   &request_info);
//    GetTimeoutRequestBodyMessage();
//    GetInternalServerErrorBodyMessage();
// };

class HttpResponse {
  public:
	typedef std::map<StatusCode, std::string> ReasonPhrase;
	static std::string                        Run(const HttpRequestResult &request_info);

  private:
	HttpResponse();
	~HttpResponse();

	static std::string        CreateHttpResponseFormat(const HttpResponseResult &response);
	static HttpResponseResult CreateHttpResponseResult(const HttpRequestResult &request_info);
	static HttpResponseResult CreateSuccessHttpResponseResult(const HttpRequestResult &request_info
	);
	static HttpResponseResult CreateErrorHttpResponseResult(const HttpRequestResult &request_info);
	static std::string
	CreateErrorBodyMessage(const std::string &status_code, const std::string &reason_phrase);
};

} // namespace http

#endif
