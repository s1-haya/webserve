#include "cgi_parse.hpp"

namespace http {
namespace cgi {
namespace {

std::string CreatePathInfo(const std::string &cgi_extension, const std::string &request_target) {
	std::string::size_type pos = request_target.find(cgi_extension);
	if (pos != std::string::npos) {
		return request_target.substr(pos + cgi_extension.length());
	}
	return "";
}

std::string TranslateToCgiPath(const std::string &request_target) {
	// from cgi_parse dir to cgi dir
	return "../../../../cgi" + request_target;
}

std::string TranslateToHtmlPath(const std::string &request_target) {
	// from cgi_parse dir to html dir
	return "../../../../html" + request_target;
}

} // namespace

Cgi::MetaMap CgiParse::CreateRequestMetaVariables(
	const HttpRequestFormat &request,
	const std::string       &cgi_script,
	const std::string       &cgi_extension
) {
	Cgi::MetaMap request_meta_variables;
	request_meta_variables["AUTH_TYPE"]         = "";
	request_meta_variables["CONTENT_LENGTH"]    = request.header_fields.at("Content-Length");
	request_meta_variables["CONTENT_TYPE"]      = request.header_fields.at("Content-Type");
	request_meta_variables["GATEWAY_INTERFACE"] = "CGI/1.1";
	request_meta_variables["PATH_INFO"] =
		CreatePathInfo(cgi_extension, request.request_line.request_target);
	request_meta_variables["PATH_TRANSLATED"] =
		TranslateToHtmlPath(request_meta_variables["PATH_INFO"]);
	request_meta_variables["QUERY_STRING"]    = "";
	request_meta_variables["REMOTE_ADDR"]     = "";
	request_meta_variables["REMOTE_HOST"]     = ""; // 追加する？
	request_meta_variables["REMOTE_IDENT"]    = "";
	request_meta_variables["REMOTE_USER"]     = "";
	request_meta_variables["REQUEST_METHOD"]  = request.request_line.method;
	request_meta_variables["SCRIPT_NAME"]     = TranslateToCgiPath(cgi_script);
	request_meta_variables["SERVER_NAME"]     = request.header_fields.at("Host");
	request_meta_variables["SERVER_PORT"]     = "8080";
	request_meta_variables["SERVER_PROTOCOL"] = request.request_line.version;
	request_meta_variables["SERVER_SOFTWARE"] = "Webserv/1.1";
	return request_meta_variables;
}

utils::Result<CgiRequest> CgiParse::Parse(
	const HttpRequestFormat &request,
	const std::string       &cgi_script,
	const std::string       &cgi_extension
) {
	CgiRequest                cgi_request;
	utils::Result<CgiRequest> result;

	try {
		cgi_request.meta_variables = CreateRequestMetaVariables(request, cgi_script, cgi_extension);
		cgi_request.body_message   = request.body_message;
		result.SetValue(cgi_request);
	} catch (const std::exception &e) {
		result.Set(false); // atで例外が投げられる
	}
	return result;
}

} // namespace cgi
} // namespace http
