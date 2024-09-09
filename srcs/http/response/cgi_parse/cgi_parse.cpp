#include "cgi_parse.hpp"

namespace http {
namespace cgi {

Cgi::MetaMap CgiParse::CreateRequestMetaVariables() {
	Cgi::MetaMap request_meta_variables;
	request_meta_variables["AUTH_TYPE"]         = "";
	request_meta_variables["CONTENT_LENGTH"]    = "26";
	request_meta_variables["CONTENT_TYPE"]      = "";
	request_meta_variables["GATEWAY_INTERFACE"] = "CGI/1.1";
	request_meta_variables["PATH_INFO"]         = "";
	request_meta_variables["PATH_TRANSLATED"]   = "";
	request_meta_variables["QUERY_STRING"]      = "";
	request_meta_variables["REMOTE_ADDR"]       = "";
	request_meta_variables["REMOTE_HOST"]       = "";
	request_meta_variables["REMOTE_IDENT"]      = "";
	request_meta_variables["REMOTE_USER"]       = "";
	request_meta_variables["REQUEST_METHOD"]    = "POST";
	request_meta_variables["SCRIPT_NAME"]       = "../../../test/apache/cgi/print_stdin.pl";
	request_meta_variables["SERVER_NAME"]       = "localhost";
	request_meta_variables["SERVER_PORT"]       = "8080";
	request_meta_variables["SERVER_PROTOCOL"]   = "HTTP/1.1";
	request_meta_variables["SERVER_SOFTWARE"]   = "Webserv/1.1";
	return request_meta_variables;
}

CgiRequest CgiParse::Parse(const std::string &http_request) {
	CgiRequest request;

	(void)http_request;
	// todo: メタ変数とボディメッセージをHTTPリクエスト情報か取得する
	// -
	// メタ変数（要相談:引数は現在HTTPリクエストの文字列やけどクライアントとサーバーの情報もいるから構造体がいい）
	// - ボディメッセージ（定数）
	request.meta_variables = CreateRequestMetaVariables();
	request.body_message   = "name=ChatGPT&message=Hello";
	return request;
}

} // namespace cgi
} // namespace http
