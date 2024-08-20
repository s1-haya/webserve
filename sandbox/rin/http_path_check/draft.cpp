#include <iostream>
#include <list>
#include <map>
#include <string>

struct RequestLine {
	std::string method;
	std::string request_target;
	std::string version;
};

typedef std::map<std::string, std::string> HeaderFields;

struct HttpRequest {
	RequestLine  request_line;
	HeaderFields header_fields;
	std::string  body_message;
};

struct LocationCon {
	std::string                          request_uri;
	std::string                          alias;
	std::string                          index;
	bool                                 autoindex;
	std::list<std::string>               allowed_methods;
	std::pair<unsigned int, std::string> redirect; // cannnot use return
	LocationCon() : autoindex(false) {}
};

typedef std::list<int>         PortList;
typedef std::list<LocationCon> LocationList;

struct ServerCon {
	PortList                             port;
	std::list<std::string>               server_names;
	LocationList                         location_con;
	std::size_t                          client_max_body_size;
	std::pair<unsigned int, std::string> error_page;
	ServerCon() : client_max_body_size(1024) {}
};

struct DtoServerInfos {
	int                    server_fd;
	std::list<std::string> server_names;
	std::string            server_port;
	LocationList           locations;
	/*以下serverでは未実装*/
	std::string                          host;
	std::size_t                          client_max_body_size;
	std::pair<unsigned int, std::string> error_page;
};

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

enum CheckStatus {
	OK,
	INVALID_HOST,
	PAYLOAD_TOO_LARGE,
	LOCATION_NOT_FOUND,
	REDIRECT
}; // rfc + 見やすいように独自で名前をつけた
// 呼び出し元でこれをチェックしてstatus codeを付ける用

struct CheckPathResult {
	std::string path; // root, index, redirectを見る
	std::string index;
	bool        autoindex;
	int         status_code; // redirectで指定
	std::string error_page_path;
	int         error_status_code; // error_pageで指定 まとめる？
	CheckStatus is_ok;
};

LocationCon CheckLocation(
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

void CheckIndex(CheckPathResult &result, const LocationCon &location) {
	if (!location.index.empty()) {
		result.index = location.index;
	}
}

void CheckAutoIndex(CheckPathResult &result, const LocationCon &location) {
	if (location.autoindex == true) {
		result.autoindex = true;
	}
}

// void CheckAllowedMethods(CheckPathResult &result, LocationCon &location, HttpRequest &request) {
// 	std::list<std::string>::iterator is_allowed_method = std::find(
// 		location.allowed_methods.begin(),
// 		location.allowed_methods.end(),
// 		request.request_line.method
// 	);
// 	// ex. allowed_method: [GET, POST], request.method GET
// 	if (is_allowed_method == location.allowed_methods.end()) {
// 		// result.is_ok = false;
// 		std::cout << "method" << std::endl;
// 	}
// }

void CheckAlias(CheckPathResult &result, const LocationCon &location) {
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

void CheckRedirect(CheckPathResult &result, const LocationCon &location) {
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

// Check LocationList
void CheckLocationList(
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

// Check Server
void CheckDTOServerInfo(
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

CheckPathResult Check(const DtoServerInfos &server_info, HttpRequest &request) {
	CheckPathResult result;

	std::memset(&result, 0, sizeof(result));
	result.is_ok = OK;
	CheckDTOServerInfo(result, server_info, request.header_fields);
	CheckLocationList(result, server_info.locations, request.request_line.request_target);
	return result;
}

// TODO: Check File existence, File authority (for post, delete) -> やらない
// TODO: indexをパスにつける -> やらない

/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
// For Test

namespace {

LocationCon BuildLocationCon(
	const std::string                          &request_uri,
	const std::string                          &alias,
	const std::string                          &index,
	bool                                        autoindex,
	const std::list<std::string>               &allowed_methods,
	const std::pair<unsigned int, std::string> &redirect
) {
	LocationCon loc;
	loc.request_uri     = request_uri;
	loc.alias           = alias;
	loc.index           = index;
	loc.autoindex       = autoindex;
	loc.allowed_methods = allowed_methods;
	loc.redirect        = redirect;
	return loc;
}

} // namespace

int main() {
	// request
	const RequestLine request_line_1 = {"GET", "/www/data/test.html", "HTTP/1.1"};
	HttpRequest       request;
	request.request_line                = request_line_1;
	request.header_fields["Host"]       = "localhost";
	request.header_fields["Connection"] = "keep-alive";

	// LocationList
	LocationList           locationlist;
	std::list<std::string> allowed_methods;
	allowed_methods.push_back("GET");
	allowed_methods.push_back("POST");
	std::pair<unsigned int, std::string> redirect;
	std::pair<unsigned int, std::string> redirect_on(301, "/");
	LocationCon                          location1 =
		BuildLocationCon("/", "", "index.html", true, allowed_methods, redirect);
	LocationCon location2 = // redirect_on
		BuildLocationCon("/www/", "", "index.html", true, allowed_methods, redirect_on);
	LocationCon location3 = // alias_on
		BuildLocationCon("/www/data/", "/var/www/", "index.html", true, allowed_methods, redirect);
	locationlist.push_back(location1);
	locationlist.push_back(location2);
	locationlist.push_back(location3);

	// DTO server_info
	DtoServerInfos         server_info;
	std::list<std::string> server_names;
	server_names.push_back("localhost");
	server_info.server_names         = server_names;
	server_info.locations            = locationlist;
	server_info.host                 = "localhost";
	server_info.client_max_body_size = 1024;
	std::pair<unsigned int, std::string> error_page(404, "/404.html");
	server_info.error_page = error_page;

	CheckPathResult result = Check(server_info, request);
	std::cout << "is_ok: " << std::boolalpha << result.is_ok << std::endl;
	std::cout << "path: " << result.path << std::endl;
	std::cout << "status_code: " << result.status_code << std::endl;
	std::cout << "index: " << result.index << std::endl;
	std::cout << "autoindex: " << std::boolalpha << result.autoindex << std::endl;
	std::cout << "error_page_code: " << result.error_status_code << std::endl;
	std::cout << "error_page_path: " << result.error_page_path << std::endl;
	if (result.is_ok != OK && result.is_ok != REDIRECT) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
