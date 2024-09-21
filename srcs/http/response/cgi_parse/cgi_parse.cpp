#include "cgi_parse.hpp"
#include "cgi_request.hpp"
#include "http_format.hpp"

namespace http {
namespace {

std::string CreatePathInfo(const std::string &cgi_extension, const std::string &request_target) {
	std::string::size_type pos = request_target.find(cgi_extension);
	if (pos != std::string::npos) {
		return request_target.substr(pos + cgi_extension.length());
	}
	return "";
}

std::string
TranslateToCgiPath(const std::string &cgi_extension, const std::string &request_target) {
	// from cgi_parse dir to cgi dir
	std::string::size_type pos = request_target.find(cgi_extension);
	if (pos != std::string::npos) { // for /aa.cgi/bb only return /aa.cgi
		return "../../../../cgi-bin" + request_target.substr(0, pos + cgi_extension.length());
	}
	return "../../../../cgi-bin" + request_target;
}

std::string TranslateToHtmlPath(const std::string &request_target) {
	// from cgi_parse dir to html dir
	return "../../../../html" + request_target;
}

} // namespace

cgi::MetaMap CgiParse::CreateRequestMetaVariables(
	const http::HttpRequestFormat &request,
	const std::string             &cgi_script,
	const std::string             &cgi_extension,
	const std::string             &server_port
) {
	cgi::MetaMap request_meta_variables;
	request_meta_variables[cgi::AUTH_TYPE]         = "";
	request_meta_variables[cgi::CONTENT_LENGTH]    = request.header_fields.at("Content-Length");
	request_meta_variables[cgi::CONTENT_TYPE]      = request.header_fields.at("Content-Type");
	request_meta_variables[cgi::GATEWAY_INTERFACE] = "CGI/1.1";
	request_meta_variables[cgi::PATH_INFO]         = CreatePathInfo(cgi_extension, cgi_script);
	request_meta_variables[cgi::PATH_TRANSLATED] =
		TranslateToHtmlPath(request_meta_variables["PATH_INFO"]);
	request_meta_variables[cgi::QUERY_STRING]    = "";
	request_meta_variables[cgi::REMOTE_ADDR]     = ""; // 追加する？
	request_meta_variables[cgi::REMOTE_HOST]     = "";
	request_meta_variables[cgi::REMOTE_IDENT]    = "";
	request_meta_variables[cgi::REMOTE_USER]     = "";
	request_meta_variables[cgi::REQUEST_METHOD]  = request.request_line.method;
	request_meta_variables[cgi::SCRIPT_NAME]     = TranslateToCgiPath(cgi_extension, cgi_script);
	request_meta_variables[cgi::SERVER_NAME]     = request.header_fields.at("Host");
	request_meta_variables[cgi::SERVER_PORT]     = server_port;
	request_meta_variables[cgi::SERVER_PROTOCOL] = request.request_line.version;
	request_meta_variables[cgi::SERVER_SOFTWARE] = "Webserv/1.1";
	return request_meta_variables;
}

utils::Result<cgi::CgiRequest> CgiParse::Parse(
	const http::HttpRequestFormat &request,
	const std::string             &cgi_script,
	const std::string             &cgi_extension,
	const std::string             &server_port
) {
	cgi::CgiRequest                cgi_request;
	utils::Result<cgi::CgiRequest> result;

	try {
		cgi_request.meta_variables =
			CreateRequestMetaVariables(request, cgi_script, cgi_extension, server_port);
		cgi_request.body_message = request.body_message;
		result.SetValue(cgi_request);
	} catch (const std::exception &e) {
		result.Set(false); // atで例外が投げられる
	}
	return result;
}

} // namespace http
