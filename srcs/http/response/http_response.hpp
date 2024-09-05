#ifndef HTTP_RESPONSE_HPP_
#define HTTP_RESPONSE_HPP_

#include "http_format.hpp"
#include "http_serverinfo_check.hpp"
#include "status_code.hpp"
#include "utils.hpp"
#include <map>
#include <string>

namespace http {

struct HttpRequestResult;
struct MockDtoClientInfos;
struct MockDtoServerInfos;
class Stat;

// HttpResponse {
// public:
//   static std::string Run(HttpRequestResult)
//   static std::string CreateTimeoutRequestResponse(HttpRequestResult)
//   static std::string CreateInternalServerErrorResponse(HttpRequestResult)
//  private:
//   static std::string        CreateHttpResponseFormat(const HttpResponseFormat &response);
//   static HttpResponseFormat CreateHttpResponseFormat(const HttpRequestResult &request_info);
//   static HttpResponseFormat CreateSuccessHttpResponseFormat(const HttpRequestResult
//   &request_info);
//   static HttpResponseFormat CreateDefaultHttpResponseFormat(const HttpRequestResult
//   &request_info);
//    GetTimeoutRequestBodyMessage();
//    GetInternalServerErrorBodyMessage();
// };

class HttpResponse {
  public:
	typedef std::map<EStatusCode, std::string> ReasonPhrase;
	static std::string
					  Run(const MockDtoClientInfos &client_info,
						  const MockDtoServerInfos &server_info,
						  const HttpRequestResult  &request_info);
	static StatusCode GetHandler(const std::string &path, std::string &body_message);
	static StatusCode PostHandler(
		const std::string &path,
		const std::string &request_body_message,
		std::string       &response_body_message
	);
	static StatusCode DeleteHandler(const std::string &path, std::string &response_body_message);
	static StatusCode MethodHandler(
		const std::string            &path,
		const std::string            &method,
		const std::list<std::string> &allow_method,
		const std::string            &request_body_message,
		std::string                  &response_body_message
	);
	static std::string CreateDefaultBodyMessageFormat(const StatusCode &status_code);
	// static std::string CreateBadRequestResponse(const HttpRequestResult &request_info);

  private:
	HttpResponse();
	~HttpResponse();

	static std::string        CreateHttpResponse(const HttpResponseFormat &response);
	static HttpResponseFormat CreateHttpResponseFormat(
		const MockDtoClientInfos &client_info,
		const MockDtoServerInfos &server_info,
		const HttpRequestResult  &request_info
	);
	static HttpResponseFormat CreateDefaultHttpResponseFormat(const StatusCode &status_code);
	static Stat               TryStat(const std::string &path, std::string &response_body_message);
	static bool
	IsAllowedMethod(const std::string &method, const std::list<std::string> &allow_method);
	static void
	SystemExceptionHandler(const utils::SystemException &e, std::string &response_body_message);
	static StatusCode FileCreationHandler(
		const std::string &path,
		const std::string &request_body_message,
		std::string       &response_body_message
	);
};

} // namespace http

#endif
