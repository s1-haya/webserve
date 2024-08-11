#include "http_path_check.hpp"
#include <iostream>

namespace http {

CheckPathResult HttpPathCheck::Check(const DtoServerInfos &server_info, HttpRequest &request) {
	CheckPathResult result;

	std::memset(&result, 0, sizeof(result));
	result.is_ok = OK;
	CheckDTOServerInfo(result, server_info, request.header_fields);
	CheckLocationList(result, server_info.locations, request.request_line.request_target);
	return result;
}

// Check Server_info
void HttpPathCheck::CheckDTOServerInfo(
	CheckPathResult &result, const DtoServerInfos &server_info, HeaderFields &header_fields
) {
	if (server_info.host != header_fields["Host"]) { // Check host_name
		result.is_ok = INVALID_HOST;
	} else if (static_cast<size_t>(std::atoi(header_fields["Content-Length"].c_str())) >
			   server_info.client_max_body_size) { // Check content_length
		result.is_ok = PAYLOAD_TOO_LARGE;
	} else if (server_info.error_page.second != "") { // Check error_page
		result.error_status_code = server_info.error_page.first;
		result.error_page_path   = server_info.error_page.second;
	}
	return;
}

// Check LocationList
void HttpPathCheck::CheckLocationList(
	CheckPathResult &result, const LocationList &locations, const std::string &request_target
) {
	if (result.is_ok != OK) {
		return;
	}
	LocationCon match_location = CheckLocation(result, locations, request_target);
	std::cout << match_location.request_uri << std::endl; // for debug
	CheckIndex(result, match_location);
	CheckAutoIndex(result, match_location);
	// CheckAllowedMethods(result, match_location, request); // なしに
	CheckAlias(result, match_location);
	CheckRedirect(result, match_location);
	return;
}

LocationCon HttpPathCheck::CheckLocation(
	CheckPathResult &result, const LocationList &locations, const std::string &request_target
) {
	LocationCon match_loc;

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
	if (match_loc.request_uri.length() == 0) {
		result.is_ok = LOCATION_NOT_FOUND;
	}
	return match_loc;
}

void HttpPathCheck::CheckIndex(CheckPathResult &result, const LocationCon &location) {
	if (!location.index.empty()) {
		result.index = location.index;
	}
}

void HttpPathCheck::CheckAutoIndex(CheckPathResult &result, const LocationCon &location) {
	if (location.autoindex == true) {
		result.autoindex = true;
	}
}

void HttpPathCheck::CheckAlias(CheckPathResult &result, const LocationCon &location) {
	if (location.alias.empty()) {
		return;
	}
	// ex. request_uri /www/ alias /var/ -> /www/を削除して/root/に
	// /www/target.html -> /var/target.html
	std::string::size_type alias_start_pos = result.path.find(location.request_uri);
	if (alias_start_pos != std::string::npos) {
		result.path.replace(alias_start_pos, location.request_uri.length(), location.alias);
	}
}

void HttpPathCheck::CheckRedirect(CheckPathResult &result, const LocationCon &location) {
	if (location.redirect.second.empty()) { // tmp
		return;
	}
	// ex. return 301 /var/data/index.html
	// /www/target.html -> /var/data/index.html
	// status code: 301
	result.status_code = location.redirect.first;
	result.path        = location.redirect.second;
	result.is_ok       = REDIRECT;
}

} // namespace http
