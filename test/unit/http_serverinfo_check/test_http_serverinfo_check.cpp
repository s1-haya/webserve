#include "http_message.hpp"
#include "http_serverinfo_check.hpp"
#include "utils.hpp"
#include "virtual_server.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>

namespace server {

typedef std::list<const VirtualServer *> VirtualServerAddrList;

}

namespace {

using namespace http;

/*==========================================================*/ // 消す

MockLocationCon BuildLocationCon(
	const std::string                          &request_uri,
	const std::string                          &alias,
	const std::string                          &index,
	bool                                        autoindex,
	const std::list<std::string>               &allowed_methods,
	const std::pair<unsigned int, std::string> &redirect,
	const std::string                          &cgi_extension    = "",
	const std::string                          &upload_directory = ""
) {
	MockLocationCon loc;
	loc.request_uri      = request_uri;
	loc.alias            = alias;
	loc.index            = index;
	loc.autoindex        = autoindex;
	loc.allowed_methods  = allowed_methods;
	loc.redirect         = redirect;
	loc.cgi_extension    = cgi_extension;
	loc.upload_directory = upload_directory;
	return loc;
}

MockDtoServerInfos BuildMockDtoServerInfos() {
	// LocationList
	LocationList           locationlist;
	std::list<std::string> allowed_methods;
	allowed_methods.push_back("GET");
	allowed_methods.push_back("POST");
	std::pair<unsigned int, std::string> redirect;
	std::pair<unsigned int, std::string> redirect_on(301, "/");
	MockLocationCon                      location1 =
		BuildLocationCon("/", "", "index.html", false, allowed_methods, redirect);
	MockLocationCon location2 = // redirect_on
		BuildLocationCon("/www/", "", "index.html", true, allowed_methods, redirect_on);
	MockLocationCon location3 = // alias_on
		BuildLocationCon("/www/data/", "/var/www/", "index.html", true, allowed_methods, redirect);
	MockLocationCon location4 = // cgi, upload_directory
		BuildLocationCon(
			"/web/", "", "index.htm", false, allowed_methods, redirect, ".php", "/data/"
		);
	locationlist.push_back(location1);
	locationlist.push_back(location2);
	locationlist.push_back(location3);
	locationlist.push_back(location4);

	// DTO server_info
	MockDtoServerInfos     server_info;
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

/*==========================================================*/ // 消す

server::Location BuildLocation(
	const std::string                          &request_uri,
	const std::string                          &alias,
	const std::string                          &index,
	bool                                        autoindex,
	const std::list<std::string>               &allowed_methods,
	const std::pair<unsigned int, std::string> &redirect,
	const std::string                          &cgi_extension    = "",
	const std::string                          &upload_directory = ""
) {
	server::Location loc;
	loc.request_uri      = request_uri;
	loc.alias            = alias;
	loc.index            = index;
	loc.autoindex        = autoindex;
	loc.allowed_methods  = allowed_methods;
	loc.redirect         = redirect;
	loc.cgi_extension    = cgi_extension;
	loc.upload_directory = upload_directory;
	return loc;
}

server::VirtualServer BuildVirtualServer1() {
	// LocationList
	typedef std::list<server::Location> LocationList;
	LocationList                        locationlist;
	std::list<std::string>              allowed_methods;
	allowed_methods.push_back("GET");
	allowed_methods.push_back("POST");
	std::pair<unsigned int, std::string> redirect;
	std::pair<unsigned int, std::string> redirect_on(301, "/");
	server::Location                     location1 =
		BuildLocation("/", "", "index.html", false, allowed_methods, redirect);
	server::Location location2 = // redirect_on
		BuildLocation("/www/", "", "index.html", true, allowed_methods, redirect_on);
	server::Location location3 = // alias_on
		BuildLocation("/www/data/", "/var/www/", "index.html", true, allowed_methods, redirect);
	server::Location location4 = // cgi, upload_directory
		BuildLocation("/web/", "", "index.htm", false, allowed_methods, redirect, ".php", "/data/");
	locationlist.push_back(location1);
	locationlist.push_back(location2);
	locationlist.push_back(location3);
	locationlist.push_back(location4);

	server::VirtualServer                 virtual_server;
	server::VirtualServer::ServerNameList server_names;
	server_names.push_back("localhost");
	server::VirtualServer::HostPortList host_ports;
	host_ports.push_back(std::make_pair("localhost", 8080));
	server::VirtualServer::ErrorPage error_page(404, "/404.html");

	return server::VirtualServer(server_names, locationlist, host_ports, 1024, error_page);
}

// ==================== Test汎用 ==================== //

int GetTestCaseNum() {
	static int test_case_num = 0;
	++test_case_num;
	return test_case_num;
}

void PrintOk() {
	std::cout << utils::color::GREEN << GetTestCaseNum() << ".[OK]" << utils::color::RESET
			  << std::endl;
}

void PrintNg() {
	std::cerr << utils::color::RED << GetTestCaseNum() << ".[NG] " << utils::color::RESET
			  << std::endl;
}

template <typename T>
void IsSame(const T &a, const T &b, const char *file, int line) {
	if (a != b) {
		std::stringstream ss;
		ss << line;
		throw std::logic_error(std::string("Error at ") + file + ":" + ss.str());
	}
}

/**
 * @brief Output where is the error
 */
#define COMPARE(a, b) IsSame(a, b, __FILE__, __LINE__)

template <class InputIt>
InputIt Next(InputIt it, typename std::iterator_traits<InputIt>::difference_type n = 1) {
	std::advance(it, n);
	return it;
} // std::next for c++98

// ================================================= //

int Test1() {
	// request
	const RequestLine request_line = {"GET", "/", "HTTP/1.1"};
	HttpRequestFormat request;
	request.request_line              = request_line;
	request.header_fields[HOST]       = "localhost";
	request.header_fields[CONNECTION] = "keep-alive";

	MockDtoServerInfos    server_info = BuildMockDtoServerInfos();
	CheckServerInfoResult result      = HttpServerInfoCheck::Check(server_info, request);
	MockLocationCon       location    = *server_info.locations.begin(); // location1

	try {
		COMPARE(result.path, location.request_uri);
		COMPARE(result.index, location.index);
		COMPARE(result.autoindex, location.autoindex);
		COMPARE(result.allowed_methods, location.allowed_methods);
		COMPARE(result.cgi_extension, location.cgi_extension);
		COMPARE(result.upload_directory, location.upload_directory);
		COMPARE(result.redirect.GetValue(), location.redirect);
		COMPARE(result.error_page.GetValue(), server_info.error_page);
	} catch (const std::exception &e) {
		PrintNg();
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	PrintOk();
	return EXIT_SUCCESS;
}

int Test2() {
	// request
	const RequestLine request_line = {"GET", "/www/test.html", "HTTP/1.1"}; // location2(redirect)
	HttpRequestFormat request;
	request.request_line              = request_line;
	request.header_fields[HOST]       = "localhost";
	request.header_fields[CONNECTION] = "keep-alive";

	MockDtoServerInfos    server_info = BuildMockDtoServerInfos();
	CheckServerInfoResult result      = HttpServerInfoCheck::Check(server_info, request);
	MockLocationCon location = *(Next(server_info.locations.begin(), 1)); // location2(redirect)

	try {
		COMPARE(result.index, location.index);
		COMPARE(result.autoindex, location.autoindex);
		COMPARE(result.allowed_methods, location.allowed_methods);
		COMPARE(result.cgi_extension, location.cgi_extension);
		COMPARE(result.upload_directory, location.upload_directory);
		COMPARE(result.redirect.GetValue(), location.redirect);
		COMPARE(result.error_page.GetValue(), server_info.error_page);
	} catch (const std::exception &e) {
		PrintNg();
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	PrintOk();
	return EXIT_SUCCESS;
}

int Test3() {
	// request
	const RequestLine request_line = {"GET", "/www/data/test.html", "HTTP/1.1"};
	HttpRequestFormat request;
	request.request_line              = request_line;
	request.header_fields[HOST]       = "localhost";
	request.header_fields[CONNECTION] = "keep-alive";

	MockDtoServerInfos    server_info = BuildMockDtoServerInfos();
	CheckServerInfoResult result      = HttpServerInfoCheck::Check(server_info, request);
	MockLocationCon       location = *(Next(server_info.locations.begin(), 2)); // location3 (alias)

	try {
		COMPARE(result.path, location.alias + "test.html");
		COMPARE(result.index, location.index);
		COMPARE(result.autoindex, location.autoindex);
		COMPARE(result.allowed_methods, location.allowed_methods);
		COMPARE(result.cgi_extension, location.cgi_extension);
		COMPARE(result.upload_directory, location.upload_directory);
		COMPARE(result.redirect.GetValue(), location.redirect);
		COMPARE(result.error_page.GetValue(), server_info.error_page);
	} catch (const std::exception &e) {
		PrintNg();
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	PrintOk();
	return EXIT_SUCCESS;
}

int Test4() {
	// request
	const RequestLine request_line = {"GET", "/web/", "HTTP/1.1"};
	HttpRequestFormat request;
	request.request_line              = request_line;
	request.header_fields[HOST]       = "localhost";
	request.header_fields[CONNECTION] = "keep-alive";

	MockDtoServerInfos    server_info = BuildMockDtoServerInfos();
	CheckServerInfoResult result      = HttpServerInfoCheck::Check(server_info, request);
	MockLocationCon       location =
		*(Next(server_info.locations.begin(), 3)); // location4 (cgi, upload_directory)

	try {
		COMPARE(result.path, location.request_uri);
		COMPARE(result.index, location.index);
		COMPARE(result.autoindex, location.autoindex);
		COMPARE(result.allowed_methods, location.allowed_methods);
		COMPARE(result.cgi_extension, location.cgi_extension);
		COMPARE(result.upload_directory, location.upload_directory);
		COMPARE(result.redirect.GetValue(), location.redirect);
		COMPARE(result.error_page.GetValue(), server_info.error_page);
	} catch (const std::exception &e) {
		PrintNg();
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	PrintOk();
	return EXIT_SUCCESS;
}

} // namespace

int main() {
	int ret = EXIT_SUCCESS;

	ret |= Test1();
	ret |= Test2();
	ret |= Test3();
	ret |= Test4();
	return ret;
}
