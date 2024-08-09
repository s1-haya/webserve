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

typedef std::list<int>         PortList;
typedef std::list<LocationCon> LocationList;

struct LocationCon {
	std::string                          request_uri;
	std::string                          alias;
	std::string                          index;
	bool                                 autoindex;
	std::list<std::string>               allowed_methods;
	std::pair<unsigned int, std::string> redirect; // cannnot use return
	LocationCon() : autoindex(false) {}
};

struct ServerCon {
	PortList                             port;
	std::list<std::string>               server_names;
	LocationList                         location_con;
	std::size_t                          client_max_body_size;
	std::pair<unsigned int, std::string> error_page;
	ServerCon() : client_max_body_size(1024) {}
};

class CheckConfig {
  private:
	/* data */
  public:
	CheckConfig(/* args */);
	~CheckConfig();
};

CheckConfig::CheckConfig(/* args */) {}

CheckConfig::~CheckConfig() {}

struct CheckConfigResult {
	std::string path; // root, index, redirectを見る
	bool        autoindex;
	int         status_code; // redirectで指定
	std::string error_page_path;
	int         error_status_code; // error_pageで指定 まとめる？
	bool        is_ok;             // Result型で受け渡し？
};

// まずはLocationをチェックする部分を作成
CheckConfigResult
CheckLocationList(std::string server_name, LocationList &locations, HttpRequest &request) {
	CheckConfigResult result;

	result.is_ok = true;
	try {
		if (request.header_fields["host"] !=
			server_name) { // TODO: CheckServer(一個上の呼び出し元)に移す
			throw std::runtime_error("invalid server_name");
		}
		LocationCon match_location = CheckLocation(result, locations, request);
		CheckAutoIndex(result, match_location, request);
		CheckAllowedMethods(result, match_location, request);
	} catch (const std::exception &e) {
		result.is_ok = false;
	}
	return result;
}

LocationCon
CheckLocation(CheckConfigResult &result, LocationList &locations, HttpRequest &request) {
	size_t      pos = 0;
	LocationCon match_loc;

	// ex. request.target /www/target.html:
	// uri = /www/, target = target.html
	// location1 /www/data/ -> first not of: 5
	// location2 / -> first not of: 1
	// Longest Match
	result.path = request.request_line.request_target;
	for (LocationList::iterator it = locations.begin(); it != locations.end(); ++it) {
		if ((*it).request_uri.find_first_not_of(request.request_line.request_target) > pos) {
			match_loc = *it;
		}
	}
	if (pos == 0) {
		throw std::runtime_error("no match Location");
	} else {
		return match_loc;
	}
}

void CheckAutoIndex(CheckConfigResult &result, LocationCon &location, HttpRequest &request) {
	if (location.autoindex == true) {
		result.autoindex = true;
	}
}

void CheckAllowedMethods(CheckConfigResult &result, LocationCon &location, HttpRequest &request) {
	std::list<std::string>::iterator is_allowed_method = std::find(
		location.allowed_methods.begin(),
		location.allowed_methods.end(),
		request.request_line.method
	);
	// ex. allowed_method: [GET, POST], request.method GET
	if (is_allowed_method == location.allowed_methods.end()) {
		result.is_ok = false;
	}
}

void CheckAlias(CheckConfigResult &result, LocationCon &location, HttpRequest &request) {
	if (location.alias == "") {
		return;
	}
	// ex. request_uri /www/ alias /var/ -> /www/を削除して/root/に
	// /www/target.html -> /var/target.html
	std::string::size_type alias_start_pos = result.path.find(location.request_uri);
	if (alias_start_pos != std::string::npos) {
		result.path.replace(alias_start_pos, location.request_uri.length(), location.alias);
	}
}

void CheckRedirect(CheckConfigResult &result, LocationCon &location, HttpRequest &request) {
	if (location.redirect.second != "") { // tmp
		return;
	}
	// ex. return 301 /var/data/index.html
	// /www/target.html -> /var/data/index.html
	// status code: 301
	result.status_code = location.redirect.first;
	result.path        = location.redirect.second;
}

void CheckErrorPage(CheckConfigResult &result, LocationCon &location, HttpRequest &request) {
	// TODO: check serverCon?
}

int main() {}
