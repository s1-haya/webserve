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
//   static HttpResponseFormat CreateHttpResponseResult(const HttpRequestResult &request_info);
//   static HttpResponseFormat CreateSuccessHttpResponseResult(const HttpRequestResult
//   &request_info);
//   static HttpResponseFormat CreateErrorHttpResponseResult(const HttpRequestResult
//   &request_info);
//    GetTimeoutRequestBodyMessage();
//    GetInternalServerErrorBodyMessage();
// };

class HttpResponse {
  public:
	typedef std::map<EStatusCode, std::string> ReasonPhrase;
	static std::string                         Run(const HttpRequestResult &request_info);
	static std::string                         TmpRun(
								const MockDtoClientInfos &client_info,
								const MockDtoServerInfos &server_info,
								const HttpRequestResult  &request_info
							);
	static StatusCode GetHandler(const std::string &path, std::string &body_message);
	static StatusCode PostHandler(
		const std::string &path,
		const std::string &request_body_message,
		std::string       &response_body_message
	);
	static StatusCode DeleteHandler(const std::string &path, std::string &response_body_message);
	static StatusCode
	MethodHandler(const CheckServerInfoResult &server_info, const std::string &method);
	static std::string CreateDefaultBodyMessageFormat(const StatusCode &status_code);

  private:
	HttpResponse();
	~HttpResponse();

	static std::string        CreateHttpResponseFormat(const HttpResponseFormat &response);
	static HttpResponseFormat CreateHttpResponseResult(const HttpRequestResult &request_info);
	static HttpResponseFormat TmpCreateHttpResponseResult(
		const MockDtoClientInfos &client_info,
		const MockDtoServerInfos &server_info,
		const HttpRequestResult  &request_info
	);
	static HttpResponseFormat CreateSuccessHttpResponseResult(const HttpRequestResult &request_info
	);
	static HttpResponseFormat CreateErrorHttpResponseResult(const StatusCode &status_code);
	static Stat               TryStat(const std::string &path, std::string &response_body_message);
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
