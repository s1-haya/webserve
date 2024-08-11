#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Location {
	std::string path;
	std::string root;

	Location(const std::string &p, const std::string &r) : path(p), root(r) {}
};

// リクエストURIに最も一致するLocationを選択する関数
const Location *
select_location(const std::vector<Location> &locations, const std::string &request_uri) {
	const Location *selected_location  = NULL;
	size_t          max_matched_length = 0;

	for (std::vector<Location>::const_iterator it = locations.begin(); it != locations.end();
		 ++it) {
		if (request_uri.find(it->path) == 0) { // プレフィックスマッチ
			if (it->path.length() > max_matched_length) {
				max_matched_length = it->path.length();
				selected_location  = &(*it);
			}
		}
	}

	return selected_location;
}

int main() {
	// Locationの設定
	std::vector<Location> locations;
	locations.push_back(Location("/", "/usr/share/nginx/html"));
	locations.push_back(Location("/test", "/usr/share/nginx/test"));
	locations.push_back(Location("/images", "/usr/share/nginx/images"));

	// テスト用リクエストURI
	std::string request_uri = "/test/page.html";

	// Locationの選択
	const Location *selected_location = select_location(locations, request_uri);

	if (selected_location) {
		std::cout << "Selected location path: " << selected_location->path << std::endl;
		std::cout << "Root directory: " << selected_location->root << std::endl;
	} else {
		std::cout << "No matching location found." << std::endl;
	}

	return 0;
}
