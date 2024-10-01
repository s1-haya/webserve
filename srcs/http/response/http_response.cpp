#include "http_response.hpp"
#include "cgi.hpp"
#include "cgi_parse.hpp"
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
	const http::ClientInfos             &client_info,
	const server::VirtualServerAddrList &server_info,
	const HttpRequestResult             &request_info,
	CgiResult                           &cgi_result
) {
	HttpResponseFormat response =
		CreateHttpResponseFormat(client_info, server_info, request_info, cgi_result);
	if (cgi_result.is_cgi) {
		return "";
	}
	return CreateHttpResponse(response);
}

// todo: HttpResponseFormat HttpResponse::CreateHttpResponseFormat(const HttpRequestResult
// &request_info) 作成
HttpResponseFormat HttpResponse::CreateHttpResponseFormat(
	const http::ClientInfos             &client_info,
	const server::VirtualServerAddrList &server_info,
	const HttpRequestResult             &request_info,
	CgiResult                           &cgi_result
) {
	StatusCode   status_code(OK);
	HeaderFields response_header_fields = InitResponseHeaderFields(request_info);
	std::string  response_body_message;
	try {
		const CheckServerInfoResult &server_info_result =
			HttpServerInfoCheck::Check(server_info, request_info.request);
		if (server_info_result.redirect.IsOk()) {
			return HandleRedirect(response_header_fields, server_info_result);
		}
		if (IsCgi(
				server_info_result.cgi_extension,
				server_info_result.path,
				request_info.request.request_line.method,
				server_info_result.allowed_methods
			)) {
			// これはパースした結果
			utils::Result<cgi::CgiRequest> cgi_parse_result = CgiParse::Parse(
				request_info.request,
				server_info_result.path,
				server_info_result.cgi_extension,
				utils::ToString(client_info.listen_server_port),
				client_info.ip
			);
			if (!cgi_parse_result.IsOk()) { // parserが直でthrowするように変更か
				throw HttpException("CGI Parse Error", StatusCode(BAD_REQUEST));
			}
			cgi_result.is_cgi      = true;
			cgi_result.cgi_request = cgi_parse_result.GetValue();
		} else {
			status_code = Method::Handler(
				server_info_result.path,
				request_info.request.request_line.method,
				server_info_result.allowed_methods,
				request_info.request.body_message,
				response_body_message,
				response_header_fields,
				server_info_result.index,
				server_info_result.autoindex
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
		std::cerr << utils::color::GRAY << "Debug [" << e.what() << "]" << utils::color::RESET
				  << std::endl;
		status_code                            = e.GetStatusCode();
		response_body_message                  = CreateDefaultBodyMessage(status_code);
		response_header_fields[CONTENT_LENGTH] = utils::ToString(response_body_message.length());
	}
	return HttpResponseFormat(
		StatusLine(HTTP_VERSION, status_code.GetStatusCode(), status_code.GetReasonPhrase()),
		response_header_fields,
		response_body_message
	);
}

std::string HttpResponse::CreateDefaultBodyMessage(const StatusCode &status_code) {
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
	response_header_fields[SERVER]       = SERVER_VERSION;
	response_header_fields[CONTENT_TYPE] = TEXT_HTML;
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
	const Method::AllowMethods &allowed_methods
) {
	// cgi_extensionがあるかどうか
	if (cgi_extension.empty()) {
		return false;
	}
	// pathがcgi_extensionで設定された拡張子かどうか
	// falseの場合はpathに普通のリクエストとして送られる
	if (cgi_extension != GetExtension(path)) {
		return false;
	}
	// methodがGETかPOSTかつallow_methodかどうか
	if (!(Method::IsAllowedMethod(method, allowed_methods)) || (method != GET && method != POST)) {
		return false;
	}
	return true;
}

HttpResponseFormat HttpResponse::HandleRedirect(
	HeaderFields &response_header_fields, const CheckServerInfoResult &server_info_result
) {
	// httpがない場合は現在のサーバーに送り、httpがある場合はそのまま
	const std::string prefix = "http://";
	const std::string url    = server_info_result.redirect.GetValue().second;
	if (url.compare(0, prefix.size(), prefix) == 0) {
		response_header_fields[LOCATION] = server_info_result.redirect.GetValue().second;
	} else {
		response_header_fields[LOCATION] = prefix + server_info_result.host_name + url;
	}
	utils::Debug("Redirect", response_header_fields[LOCATION]);
	// サーバーで用意しているステータス以外を指定した場合はbodyやphraseを返さない
	if (server_info_result.redirect.GetValue().first == MOVED_PERMANENTLY) {
		throw HttpException("Moved Permanently", StatusCode(MOVED_PERMANENTLY));
	} else {
		response_header_fields[CONTENT_LENGTH] = "0";
		const std::string redirect_status_code =
			utils::ToString(server_info_result.redirect.GetValue().first);
		return HttpResponseFormat(
			StatusLine(HTTP_VERSION, redirect_status_code, ""), response_header_fields, ""
		);
	}
}

std::string HttpResponse::CreateErrorResponse(const StatusCode &status_code) {
	HttpResponseFormat response;
	response.status_line =
		StatusLine(HTTP_VERSION, status_code.GetStatusCode(), status_code.GetReasonPhrase());
	response.header_fields[SERVER]         = SERVER_VERSION;
	response.header_fields[CONTENT_TYPE]   = TEXT_HTML;
	response.header_fields[CONNECTION]     = CLOSE;
	response.body_message                  = CreateDefaultBodyMessage(status_code);
	response.header_fields[CONTENT_LENGTH] = utils::ToString(response.body_message.length());
	return CreateHttpResponse(response);
}

std::string HttpResponse::GetResponseFromCgi(
	const cgi::CgiResponse &cgi_response, const HttpRequestResult &request_info
) {
	StatusCode  status_code(OK);
	std::string response_body_message = cgi_response.response;

	HeaderFields response_header_fields;
	response_header_fields[SERVER]         = SERVER_VERSION;
	response_header_fields[CONTENT_TYPE]   = cgi_response.content_type;
	response_header_fields[CONTENT_LENGTH] = utils::ToString(response_body_message.length());
	if (IsConnectionKeep(request_info.request.header_fields)) {
		response_header_fields[CONNECTION] = KEEP_ALIVE;
	} else {
		response_header_fields[CONNECTION] = CLOSE;
	} // InitResponseHeaderFields が完成したらそれを使う

	HttpResponseFormat response_format(
		StatusLine(HTTP_VERSION, status_code.GetStatusCode(), status_code.GetReasonPhrase()),
		response_header_fields,
		response_body_message
	);

	return CreateHttpResponse(response_format);
}

} // namespace http
