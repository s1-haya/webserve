#include "http_path_check.hpp"
#include <cstdlib>
#include <iostream>

using namespace http;

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

DtoServerInfos BuildDtoServerInfos() {
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

	return server_info;
}

int Test1() {
	// request
	const RequestLine request_line = {"GET", "/", "HTTP/1.1"}; // location1
	HttpRequestFormat request;
	request.request_line                = request_line;
	request.header_fields["Host"]       = "localhost";
	request.header_fields["Connection"] = "keep-alive";

	DtoServerInfos  server_info = BuildDtoServerInfos();
	CheckPathResult result      = HttpPathCheck::Check(server_info, request);
	std::cout << "is_ok: " << std::boolalpha << result.is_ok << std::endl;
	std::cout << "path: " << result.path << std::endl;
	std::cout << "status_code: " << result.status_code << std::endl;
	std::cout << "index: " << result.index << std::endl;
	std::cout << "autoindex: " << std::boolalpha << result.autoindex << std::endl;
	std::cout << "error_page_code: " << result.error_status_code << std::endl;
	std::cout << "error_page_path: " << result.error_page_path << std::endl;
	std::cout << std::endl;
	if (result.is_ok != OK && result.is_ok != REDIRECT) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int Test2() {
	// request
	const RequestLine request_line = {"GET", "/www/test.html", "HTTP/1.1"}; // location2(redirect)
	HttpRequestFormat request;
	request.request_line                = request_line;
	request.header_fields["Host"]       = "localhost";
	request.header_fields["Connection"] = "keep-alive";

	DtoServerInfos  server_info = BuildDtoServerInfos();
	CheckPathResult result      = HttpPathCheck::Check(server_info, request);
	std::cout << "is_ok: " << std::boolalpha << result.is_ok << std::endl;
	std::cout << "path: " << result.path << std::endl;
	std::cout << "status_code: " << result.status_code << std::endl;
	std::cout << "index: " << result.index << std::endl;
	std::cout << "autoindex: " << std::boolalpha << result.autoindex << std::endl;
	std::cout << "error_page_code: " << result.error_status_code << std::endl;
	std::cout << "error_page_path: " << result.error_page_path << std::endl;
	std::cout << std::endl;
	if (result.is_ok != OK && result.is_ok != REDIRECT) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int Test3() {
	// request
	const RequestLine request_line = {
		"GET", "/www/data/test.html", "HTTP/1.1"
	}; // location3 (alias)
	HttpRequestFormat request;
	request.request_line                = request_line;
	request.header_fields["Host"]       = "localhost";
	request.header_fields["Connection"] = "keep-alive";

	DtoServerInfos  server_info = BuildDtoServerInfos();
	CheckPathResult result      = HttpPathCheck::Check(server_info, request);
	std::cout << "is_ok: " << std::boolalpha << result.is_ok << std::endl;
	std::cout << "path: " << result.path << std::endl;
	std::cout << "status_code: " << result.status_code << std::endl;
	std::cout << "index: " << result.index << std::endl;
	std::cout << "autoindex: " << std::boolalpha << result.autoindex << std::endl;
	std::cout << "error_page_code: " << result.error_status_code << std::endl;
	std::cout << "error_page_path: " << result.error_page_path << std::endl;
	std::cout << std::endl;
	if (result.is_ok != OK && result.is_ok != REDIRECT) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

} // namespace

// c++ -std=c++98 -Wall -Wextra -Werror -pedantic main.cpp http_path_check.cpp
int main() {
	int ret = EXIT_SUCCESS;

	ret |= Test1();
	ret |= Test2();
	ret |= Test3();
	return ret;
}
