#ifndef HTTP_RESPONSE_HPP_
#define HTTP_RESPONSE_HPP_

#include "http_format.hpp"
#include "http_method.hpp"
#include "http_serverinfo_check.hpp"
#include "status_code.hpp"
#include "utils.hpp"
#include <map>
#include <string>

namespace http {

struct HttpRequestResult;
struct ClientInfos;
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
					   Run(const ClientInfos                   &client_info,
						   const server::VirtualServerAddrList &server_info,
						   const HttpRequestResult             &request_info);
	static std::string CreateDefaultBodyMessageFormat(const StatusCode &status_code);
	static bool        IsConnectionKeep(const HeaderFields &request_header_fields);
	static std::string CreateBadRequestResponse(const HttpRequestResult &request_info);

  private:
	HttpResponse();
	~HttpResponse();

	static std::string        CreateHttpResponse(const HttpResponseFormat &response);
	static HttpResponseFormat CreateHttpResponseFormat(
		const ClientInfos                   &client_info,
		const server::VirtualServerAddrList &server_info,
		const HttpRequestResult             &request_info
	);
	static HeaderFields InitResponseHeaderFields(const HttpRequestResult &request_info);
	static bool         IsCgi(
				const std::string          &cgi_extension,
				const std::string          &path,
				const std::string          &method,
				const Method::AllowMethods &allowed_methods,
				const std::string          &upload_directory
			);
};

} // namespace http

#endif
