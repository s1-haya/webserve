#ifndef HTTP_RESPONSE_HPP_
#define HTTP_RESPONSE_HPP_

#include "http_format.hpp"
#include "status_code.hpp"
#include "utils.hpp"
#include <map>
#include <string>

namespace http {

struct HttpRequestResult;
struct MockDtoClientInfos;
struct MockDtoServerInfos;

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
//   static HttpResponseFormat CreateErrorHttpResponseFormat(const HttpRequestResult
//   &request_info);
//    GetTimeoutRequestBodyMessage();
//    GetInternalServerErrorBodyMessage();
// };

class HttpResponse {
  public:
	typedef std::map<StatusCode, std::string> ReasonPhrase;
	static std::string                        Run(const HttpRequestResult &request_info);
	static std::string                        TmpRun(
							   const MockDtoClientInfos &client_info,
							   const MockDtoServerInfos &server_info,
							   HttpRequestResult        &request_info
						   );
	static void GetHandler(const std::string &path, std::string &body_message);
	static void PostHandler(
		const std::string &path,
		const std::string &request_body_message,
		std::string       &response_body_message
	);
	static void DeleteHandler(const std::string &path, std::string &response_body_message);

  private:
	HttpResponse();
	~HttpResponse();

	static std::string        CreateHttpResponse(const HttpResponseFormat &response);
	static HttpResponseFormat CreateHttpResponseFormat(const HttpRequestResult &request_info);
	static HttpResponseFormat TmpCreateHttpResponseFormat(
		const MockDtoClientInfos &client_info,
		const MockDtoServerInfos &server_info,
		HttpRequestResult        &request_info
	);
	static HttpResponseFormat CreateSuccessHttpResponseFormat(const HttpRequestResult &request_info
	);
	static HttpResponseFormat CreateErrorHttpResponseFormat(const HttpRequestResult &request_info);
	static std::string        CreateDefaultBodyMessageFormat(
			   const std::string &status_code, const std::string &reason_phrase
		   );
	static void
	SystemExceptionHandler(const utils::SystemException &e, std::string &response_body_message);
	static void FileCreationHandler(
		const std::string &path,
		const std::string &request_body_message,
		std::string       &response_body_message
	);
};

} // namespace http

#endif
