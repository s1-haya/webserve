#include "http_response.hpp"
#include "client_infos.hpp"
#include "http_exception.hpp"
#include "http_message.hpp"
#include "http_parse.hpp"
#include "server_infos.hpp"
#include <iostream>
#include <sstream>

namespace http {

std::string HttpResponse::Run(
	const MockDtoClientInfos &client_info,
	const MockDtoServerInfos &server_info,
	const HttpRequestResult  &request_info
) {
	HttpResponseFormat response =
		CreateHttpResponseFormat(client_info, server_info, request_info);
	return CreateHttpResponse(response);
}

// todo: HttpResponseFormat HttpResponse::CreateHttpResponseFormat(const HttpRequestResult
// &request_info) 作成
HttpResponseFormat HttpResponse::CreateHttpResponseFormat(
	const MockDtoClientInfos &client_info,
	const MockDtoServerInfos &server_info,
	const HttpRequestResult  &request_info
) {
	// todo: InitHeaderFields(最終的にはtryの外側でinitする/try,
	// catch両方header_fieldsを使用するため)
	try {
		HttpResponseFormat           result;
		const CheckServerInfoResult &server_info_result =
			HttpServerInfoCheck::Check(server_info, request_info.request);
		// todo: if redirect
		// if (server_info_result.redirect.IsOk()) {
		// 	result = RedirectHandler();
		// todo: IsCgi()
		// - path
		// - cgi_extension
		// - method allowed
		// } else if (IsCgi()) {
		// todo: cgi実行
		// cgi::Run()
		// -> Internal　Server Errorを投げる可能性あり
		// result = CgiToServerHandler();
		// } else {
		// }
		//     return CreateSuccessResponseResult();
		(void)client_info;
		(void)server_info_result;
		std::string       response_body_message;
		const StatusCode &status_code = MethodHandler(
			server_info_result.path,
			request_info.request.request_line.method,
			server_info_result.allowed_methods,
			request_info.request.body_message,
			response_body_message
		);
		return result;
	} catch (const HttpException &e) {
		// ステータスコードが300番台以上の場合
		// feature: header_fieldとerror_pageとの関連性がわかり次第変更あり
		// 返り値: response 引数:error_page, status_code
		// todo: error_page status_code classに対応
		// if (server_info.error_page.IsOk() &&
		// 	status_code.GetEStatusCode() == server_info.error_page.GetValue().first) {
		// 	response_message = ReadFile(server_info.error_page.GetValue().second);
		// 	// check the path of error_page
		// }
		return CreateDefaultHttpResponseFormat(e.GetStatusCode());
	}
}

std::string HttpResponse::CreateDefaultBodyMessageFormat(const StatusCode &status_code) {
	std::ostringstream body_message;
	body_message << "<html>" << CRLF << "<head><title>" << status_code.GetStatusCode() << SP
				 << status_code.GetReasonPhrase() << "</title></head>" << CRLF << "<body>" << CRLF
				 << "<center><h1>" << status_code.GetStatusCode() << SP
				 << status_code.GetReasonPhrase() << "</h1></center>" << CRLF << "<hr><center>"
				 << SERVER_VERSION << "</center>" << CRLF << "</body>" << CRLF << "</html>" << CRLF;
	return body_message.str();
}

// mock
HttpResponseFormat HttpResponse::CreateDefaultHttpResponseFormat(const StatusCode &status_code) {
	HttpResponseFormat response;
	response.status_line.version           = HTTP_VERSION;
	response.status_line.status_code       = status_code.GetStatusCode();
	response.status_line.reason_phrase     = status_code.GetReasonPhrase();
	response.header_fields[CONTENT_TYPE]   = "text/html";
	response.header_fields[SERVER]         = SERVER_VERSION;
	response.header_fields[CONNECTION]     = "close";
	response.body_message                  = CreateDefaultBodyMessageFormat(status_code);
	response.header_fields[CONTENT_LENGTH] = utils::ToString(response.body_message.length());
	return response;
}

std::string HttpResponse::CreateHttpResponse(const HttpResponseFormat &response) {
	std::ostringstream response_stream;
	response_stream << response.status_line.version << SP << response.status_line.status_code << SP
					<< response.status_line.reason_phrase << CRLF;
	typedef HeaderFields::const_iterator It;
	for (It it = response.header_fields.begin(); it != response.header_fields.end(); ++it) {
		response_stream << it->first << ":" << SP << it->second << CRLF;
	}
	response_stream << CRLF;
	response_stream << response.body_message;
	return response_stream.str();
}

// std::string HttpResponse::CreateBadRequestResponse(const HttpRequestResult &request_info) {
// 	HttpResponseFormat response = CreateDefaultHttpResponseFormat(request_info.status_code);
// 	return CreateHttpResponse(response);
// }

} // namespace http
