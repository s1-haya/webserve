#include "http_response.hpp"
#include "client_infos.hpp"
#include "http_exception.hpp"
#include "http_message.hpp"
#include "http_parse.hpp"
#include <iostream>
#include <sstream>

namespace {

std::string GetExtension(const std::string &path) {
	size_t pos = path.find_last_of('.');

	if (pos == std::string::npos || pos == path.length() - 1) {
		return "";
	}
	return path.substr(pos + 1);
}

} // namespace

namespace http {

std::string HttpResponse::Run(
	const MockDtoClientInfos &client_info,
	const server::VirtualServerAddrList &server_info,
	const HttpRequestResult  &request_info
) {
	HttpResponseFormat response = CreateHttpResponseFormat(client_info, server_info, request_info);
	return CreateHttpResponse(response);
}

// todo: HttpResponseFormat HttpResponse::CreateHttpResponseFormat(const HttpRequestResult
// &request_info) 作成
HttpResponseFormat HttpResponse::CreateHttpResponseFormat(
	const MockDtoClientInfos &client_info,
	const server::VirtualServerAddrList &server_info,
	const HttpRequestResult  &request_info
) {
	StatusCode   status_code(OK);
	HeaderFields response_header_fields = InitResponseHeaderFields(request_info);
	std::string  response_body_message;
	try {
		const CheckServerInfoResult &server_info_result =
			HttpServerInfoCheck::Check(server_info, request_info.request);
		// todo: if redirect
		// if (server_info_result.redirect.IsOk()) {
		// 	result = RedirectHandler();
		if (IsCgi(
				server_info_result.cgi_extension,
				server_info_result.path,
				request_info.request.request_line.method,
				server_info_result.allowed_methods,
				server_info_result.upload_directory
			)) {
			// todo: cgi実行
			// cgi::Run()
			// -> Internal　Server Errorを投げる可能性あり
			// status_code = CgiToServerHandler(header_fields, response_body_message);
			(void)client_info;
			(void)server_info_result;
			(void)status_code;
		} else {
			status_code = Method::Handler(
				server_info_result.path,
				request_info.request.request_line.method,
				server_info_result.allowed_methods,
				request_info.request.body_message,
				response_body_message,
				response_header_fields
			);
		}
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
		// for debug
		std::cout << utils::color::GRAY << "Debug [" << e.what() << "]" << utils::color::RESET << std::endl;
		status_code           = e.GetStatusCode();
		response_body_message = CreateDefaultBodyMessageFormat(status_code);
		response_header_fields[CONTENT_LENGTH] = utils::ToString(response_body_message.length());
	}
	return HttpResponseFormat(
		StatusLine(HTTP_VERSION, status_code.GetStatusCode(), status_code.GetReasonPhrase()),
		response_header_fields,
		response_body_message
	);
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

HeaderFields HttpResponse::InitResponseHeaderFields(const HttpRequestResult &request_info) {
	HeaderFields response_header_fields;
	response_header_fields[SERVER] = SERVER_VERSION;
	(void)request_info;
	// todo: request_infoから情報取得
	// GetContentType(request_info);
	// GetConnection(request_info);
	response_header_fields[CONTENT_TYPE] = "test/html";
	if (IsConnectionKeep(request_info.request.header_fields)) {
		response_header_fields[CONNECTION] = KEEP_ALIVE;
	} else {
		response_header_fields[CONNECTION] = CLOSE;
	}
	return response_header_fields;
}

bool HttpResponse::IsConnectionKeep(const HeaderFields &request_header_fields) {
	HeaderFields::const_iterator it = request_header_fields.find(CONNECTION);
	return it == request_header_fields.end() || it->second == KEEP_ALIVE;
}

bool HttpResponse::IsCgi(
	const std::string          &cgi_extension,
	const std::string          &path,
	const std::string          &method,
	const Method::AllowMethods &allowed_methods,
	const std::string          &upload_directory
) {
	// todo:
	// cgi_extensionがあるかどうか
	if (cgi_extension.empty()) {
		return false;
	}
	// upload_directory内にpathが存在するかどうか
	// -> pathがaliasで設定される場合どうなるんだろう。
	(void)upload_directory;
	// pathがcgi_extensionで設定された拡張子かどうか
	if (cgi_extension != GetExtension(path)) {
		return false;
	}
	// methodがGETかPOSTかつallow_methodかどうか
	if (!(Method::IsAllowedMethod(method, allowed_methods)) || (method != GET && method != POST)) {
		return false;
	}
	return true;
}

// std::string HttpResponse::CreateBadRequestResponse(const HttpRequestResult &request_info) {
// 	HttpResponseFormat response = CreateDefaultHttpResponseFormat(request_info.status_code);
// 	return CreateHttpResponse(response);
// }

} // namespace http
