#include "cgi_parse.hpp"

namespace http {
namespace cgi {

Cgi::MetaMap CgiParse::CreateRequestMetaVariables(const HttpRequestFormat &request) {
	Cgi::MetaMap request_meta_variables;
	// at()の要素がない場合どこかで弾く
	request_meta_variables["AUTH_TYPE"]         = "";
	request_meta_variables["CONTENT_LENGTH"]    = request.header_fields.at("Content-Length");
	request_meta_variables["CONTENT_TYPE"]      = request.header_fields.at("Content-Type");
	request_meta_variables["GATEWAY_INTERFACE"] = "CGI/1.1";
	request_meta_variables["PATH_INFO"]         = "";
	request_meta_variables["PATH_TRANSLATED"]   = "";
	request_meta_variables["QUERY_STRING"]      = "";
	request_meta_variables["REMOTE_ADDR"]       = "";
	request_meta_variables["REMOTE_HOST"]       = "";
	request_meta_variables["REMOTE_IDENT"]      = "";
	request_meta_variables["REMOTE_USER"]       = "";
	request_meta_variables["REQUEST_METHOD"]    = request.request_line.method;
	request_meta_variables["SCRIPT_NAME"]       = "../../../test/apache/cgi/print_stdin.pl";
	request_meta_variables["SERVER_NAME"]       = request.header_fields.at("Host");
	request_meta_variables["SERVER_PORT"]       = "8080";
	request_meta_variables["SERVER_PROTOCOL"]   = request.request_line.version;
	request_meta_variables["SERVER_SOFTWARE"]   = "Webserv/1.1";
	return request_meta_variables;
}

utils::Result<CgiRequest> CgiParse::Parse(const HttpRequestFormat &request) {
	utils::Result<CgiRequest> result;
	CgiRequest                cgi_request;

	cgi_request.meta_variables = CreateRequestMetaVariables(request);
	// request.body_message   = "name=ChatGPT&message=Hello";
	return result;
}

} // namespace cgi
} // namespace http
