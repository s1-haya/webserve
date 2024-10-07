#include "cgi_parse.hpp"
#include "cgi_request.hpp"
#include "http_format.hpp"
#include "http_message.hpp"
#include "utils.hpp"

namespace http {
namespace {

std::string CreatePathInfo(const std::string &cgi_extension, const std::string &request_target) {
	std::string::size_type pos = request_target.find(cgi_extension);
	if (pos != std::string::npos) {
		return request_target.substr(pos + cgi_extension.length());
	}
	return "";
}

bool EndWith(const std::string &str, const std::string &suffix) {
	return str.size() >= suffix.size() &&
		   str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string
TranslateToScriptName(const std::string &cgi_extension, const std::string &request_target) {
	// from root path
	const std::string::size_type root_pos = request_target.find("root/cgi-bin/");
	if (root_pos != std::string::npos) {
		const std::string script_name = request_target.substr(root_pos);
		if (!EndWith(script_name, cgi_extension)) {
			const std::string::size_type extension_pos = script_name.find(cgi_extension);
			// /aa.cgi/bb の場合、/aa.cgi のみを返す
			return script_name.substr(0, extension_pos + cgi_extension.length());
		}
		return script_name;
	} else {
		// root_pos が見つからない場合は request_target をそのまま返す
		return request_target;
	}
}

std::string TranslatePathInfo(const std::string &request_target) {
	// from root path
	if (request_target.empty()) {
		return "";
	}
	return "root" + request_target;
}

std::string FindValueFromMap(const cgi::MetaMap &map, const std::string &key) {
	cgi::MetaMap::const_iterator it = map.find(key);
	if (it != map.end()) {
		return it->second;
	}
	return "";
}

} // namespace

cgi::MetaMap CgiParse::CreateRequestMetaVariables(
	const http::HttpRequestFormat &request,
	const std::string             &cgi_script,
	const std::string             &cgi_extension,
	const std::string             &server_port,
	const std::string             &client_ip
) {
	cgi::MetaMap request_meta_variables;
	request_meta_variables[cgi::AUTH_TYPE] = "";
	if (!request.body_message.empty()) {
		request_meta_variables[cgi::CONTENT_LENGTH] =
			utils::ConvertUintToStr(request.body_message.length());
		request_meta_variables[cgi::CONTENT_TYPE] =
			FindValueFromMap(request.header_fields, CONTENT_TYPE);
	} // bodyがない場合はunset
	request_meta_variables[cgi::GATEWAY_INTERFACE] = "CGI/1.1";
	request_meta_variables[cgi::PATH_INFO]         = CreatePathInfo(cgi_extension, cgi_script);
	request_meta_variables[cgi::PATH_TRANSLATED] =
		TranslatePathInfo(request_meta_variables["PATH_INFO"]);
	request_meta_variables[cgi::QUERY_STRING]    = "";
	request_meta_variables[cgi::REMOTE_ADDR]     = client_ip;
	request_meta_variables[cgi::REMOTE_HOST]     = "";
	request_meta_variables[cgi::REMOTE_IDENT]    = "";
	request_meta_variables[cgi::REMOTE_USER]     = "";
	request_meta_variables[cgi::REQUEST_METHOD]  = request.request_line.method;
	request_meta_variables[cgi::SCRIPT_NAME]     = TranslateToScriptName(cgi_extension, cgi_script);
	request_meta_variables[cgi::SERVER_NAME]     = FindValueFromMap(request.header_fields, HOST);
	request_meta_variables[cgi::SERVER_PORT]     = server_port;
	request_meta_variables[cgi::SERVER_PROTOCOL] = request.request_line.version;
	request_meta_variables[cgi::SERVER_SOFTWARE] = SERVER_VERSION;
	return request_meta_variables;
}

cgi::CgiRequest CgiParse::Parse(
	const http::HttpRequestFormat &request,
	const std::string             &cgi_script,
	const std::string             &cgi_extension,
	const std::string             &server_port,
	const std::string             &client_ip
) {
	cgi::CgiRequest cgi_request;

	cgi_request.meta_variables =
		CreateRequestMetaVariables(request, cgi_script, cgi_extension, server_port, client_ip);
	cgi_request.body_message = request.body_message;
	return cgi_request;
}

} // namespace http
