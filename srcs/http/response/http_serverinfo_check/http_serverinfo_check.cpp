#include "http_serverinfo_check.hpp"
#include "http_exception.hpp"
#include "status_code.hpp"
#include <cstdlib> // atoi
#include <iostream>

namespace http {

CheckServerInfoResult
HttpServerInfoCheck::Check(const MockDtoServerInfos &server_info, HttpRequestFormat &request) {
	CheckServerInfoResult result;

	CheckDTOServerInfo(result, server_info, request.header_fields);
	CheckLocationList(result, server_info.locations, request.request_line.request_target);
	return result;
}

// Check Server_info
void HttpServerInfoCheck::CheckDTOServerInfo(
	CheckServerInfoResult    &result,
	const MockDtoServerInfos &server_info,
	HeaderFields             &header_fields
) {
	if (static_cast<size_t>(std::atoi(header_fields["Content-Length"].c_str())) >
		server_info.client_max_body_size) { // Check content_length
		throw HttpException("Error: payload too large.", PAYLOAD_TOO_LARGE);
	} else if (!server_info.error_page.second.empty()) { // Check error_page
		result.error_page = server_info.error_page;
	}
	return;
}

// Check LocationList
void HttpServerInfoCheck::CheckLocationList(
	CheckServerInfoResult &result, const LocationList &locations, const std::string &request_target
) {
	const MockLocationCon &match_location = CheckLocation(result, locations, request_target);
	// std::cout << match_location.request_uri << std::endl; // for debug
	CheckIndex(result, match_location);
	CheckAutoIndex(result, match_location);
	CheckAlias(result, match_location);
	CheckRedirect(result, match_location);
	CheckAllowedMethods(result, match_location);
	CheckCgiExtension(result, match_location);
	CheckUploadDirectory(result, match_location);
	return;
}

const MockLocationCon HttpServerInfoCheck::CheckLocation(
	CheckServerInfoResult &result, const LocationList &locations, const std::string &request_target
) {
	MockLocationCon match_loc;

	// ex. request.target /www/target.html:
	// uri = /www/, target = target.html
	// location1 /www/data/
	// location1 /www/
	// location2 /
	// -> /www/
	result.path = request_target;
	for (LocationList::const_iterator it = locations.begin(); it != locations.end(); ++it) {
		if (request_target.find((*it).request_uri) == 0 &&
			(*it).request_uri.length() > match_loc.request_uri.length()) { // Longest Match
			match_loc = *it;
		}
	}
	if (match_loc.request_uri.empty()) {
		throw HttpException("Error: location not found", NOT_FOUND);
	}
	return match_loc;
}

void HttpServerInfoCheck::CheckIndex(
	CheckServerInfoResult &result, const MockLocationCon &location
) {
	result.index = location.index;
}

void HttpServerInfoCheck::CheckAutoIndex(
	CheckServerInfoResult &result, const MockLocationCon &location
) {
	if (location.autoindex == true) {
		result.autoindex = true;
	}
}

void HttpServerInfoCheck::CheckAlias(
	CheckServerInfoResult &result, const MockLocationCon &location
) {
	if (location.alias.empty()) {
		return;
	}
	// ex. request_uri /www/ alias /var/ -> /www/を削除して/var/に
	// /www/target.html -> /var/target.html
	std::string::size_type alias_start_pos = result.path.find(location.request_uri);
	if (alias_start_pos != std::string::npos) {
		result.path.replace(alias_start_pos, location.request_uri.length(), location.alias);
	}
}

void HttpServerInfoCheck::CheckRedirect(
	CheckServerInfoResult &result, const MockLocationCon &location
) {
	if (location.redirect.second.empty()) {
		return;
	}
	// ex. return 301 /var/data/index.html
	// /www/target.html -> /var/data/index.html
	// status code: 301
	result.redirect.Set(true, location.redirect);
}

void HttpServerInfoCheck::CheckAllowedMethods(
	CheckServerInfoResult &result, const MockLocationCon &location
) {
	result.allowed_methods = location.allowed_methods;
}

void HttpServerInfoCheck::CheckCgiExtension(
	CheckServerInfoResult &result, const MockLocationCon &location
) {
	result.cgi_extension = location.cgi_extension;
}

void HttpServerInfoCheck::CheckUploadDirectory(
	CheckServerInfoResult &result, const MockLocationCon &location
) {
	result.upload_directory = location.upload_directory;
}

} // namespace http
