#include "http_response.hpp"
#include "client_infos.hpp"
#include "http_message.hpp"
#include "http_parse.hpp"
#include "http_serverinfo_check.hpp"
#include "server_infos.hpp"
#include <iostream>
#include <sstream>

namespace http {

std::string HttpResponse::Run(const HttpRequestResult &request_info) {
	HttpResponseFormat response = CreateHttpResponseFormat(request_info);
	return CreateHttpResponse(response);
}

std::string HttpResponse::TmpRun(
	const MockDtoClientInfos &client_info,
	const MockDtoServerInfos &server_info,
	const HttpRequestResult  &request_info
) {
	HttpResponseFormat response =
		TmpCreateHttpResponseFormat(client_info, server_info, request_info);
	return CreateHttpResponse(response);
}

// todo: HttpResponseFormat HttpResponse::CreateHttpResponseFormat(const HttpRequestResult
// &request_info) 作成
HttpResponseFormat HttpResponse::TmpCreateHttpResponseFormat(
	const MockDtoClientInfos &client_info,
	const MockDtoServerInfos &server_info,
	const HttpRequestResult  &request_info
) {
	try {
		HttpResponseFormat    result;
		CheckServerInfoResult server_info_result =
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
		// 	result = MethodHandler();
		// }
		//     return CreateSuccessResponseResult();
		(void)client_info;
		(void)request_info;
		(void)server_info_result;
		return result;
	} catch (const HttpException &e) {
		// feature: header_fieldとerror_pageとの関連性がわかり次第変更あり
		return CreateErrorHttpResponseFormat(e.GetStatusCode());
	}
}

// mock
HttpResponseFormat HttpResponse::CreateHttpResponseFormat(const HttpRequestResult &request_info) {
	HttpResponseFormat response;
	if (request_info.status_code.GetEStatusCode() != http::OK) {
		response = CreateErrorHttpResponseFormat(request_info.status_code);
	} else {
		response = CreateSuccessHttpResponseFormat(request_info);
	}
	return response;
}

// mock
HttpResponseFormat
HttpResponse::CreateSuccessHttpResponseFormat(const HttpRequestResult &request_info) {
	(void)request_info;
	HttpResponseFormat response;
	response.status_line.version         = HTTP_VERSION;
	response.status_line.status_code     = "200";
	response.status_line.reason_phrase   = "OK";
	response.header_fields["Host"]       = "sawa";
	response.header_fields["Connection"] = "close";
	response.body_message = "You can't connect the dots looking forward. You can only connect the "
							"dots looking backwards";
	return response;
}

std::string HttpResponse::CreateDefaultBodyMessageFormat(const StatusCode &status_code) {
	std::ostringstream body_message;
	body_message << "<html>" << CRLF << "<head><title>" << status_code.GetStatusCode() << SP << status_code.GetReasonPhrase()
				 << "</title></head>" << CRLF << "<body>" << CRLF << "<center><h1>" << status_code.GetStatusCode()
				 << SP << status_code.GetReasonPhrase() << "</h1></center>" << CRLF << "<hr><center>"
				 << SERVER_VERSION << "</center>" << CRLF << "</body>" << CRLF << "</html>" << CRLF;
	return body_message.str();
}

// mock
HttpResponseFormat HttpResponse::CreateErrorHttpResponseFormat(const StatusCode &status_code) {
	HttpResponseFormat response;
	response.status_line.version         = HTTP_VERSION;
	response.status_line.status_code     = status_code.GetStatusCode();
	response.status_line.reason_phrase   = status_code.GetReasonPhrase();
	response.header_fields[CONTENT_TYPE] = "text/html";
	response.header_fields[SERVER]       = SERVER_VERSION;
	response.header_fields[CONNECTION]   = "close";
	response.body_message                = CreateDefaultBodyMessageFormat(status_code);
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

} // namespace http
