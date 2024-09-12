#include "http_message.hpp"
#include "http_serverinfo_check.hpp"
#include "utils.hpp"
#include "virtual_server.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>

namespace {

using namespace http;

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

server::VirtualServer *BuildVirtualServer1() {
	// LocationList
	server::VirtualServer::LocationList locationlist;
	server::Location::AllowedMethodList allowed_methods;
	allowed_methods.push_back("GET");
	allowed_methods.push_back("POST");
	server::Location::Redirect redirect;
	server::Location::Redirect redirect_on(301, "/");
	server::Location           location1 =
		BuildLocation("/", "", "index.html", false, allowed_methods, redirect);
	server::Location location2 = // redirect_on
		BuildLocation("/www/", "", "index.html", true, allowed_methods, redirect_on);
	locationlist.push_back(location1);
	locationlist.push_back(location2);

	server::VirtualServer                 virtual_server;
	server::VirtualServer::ServerNameList server_names;
	server_names.push_back("host1");
	server::VirtualServer::HostPortList host_ports;
	host_ports.push_back(std::make_pair("localhost", 8080));
	server::VirtualServer::ErrorPage error_page(404, "/404.html");

	return new server::VirtualServer(server_names, locationlist, host_ports, 1024, error_page);
}

server::VirtualServer *BuildVirtualServer2() {
	// LocationList
	server::VirtualServer::LocationList locationlist;
	server::Location::AllowedMethodList allowed_methods;
	allowed_methods.push_back("GET");
	allowed_methods.push_back("DELETE");
	server::Location::Redirect redirect;
	server::Location           location1 = // alias_on
		BuildLocation("/www/data/", "/var/www/", "index.html", true, allowed_methods, redirect);
	server::Location location2 = // cgi, upload_directory
		BuildLocation("/web/", "", "index.htm", false, allowed_methods, redirect, ".php", "/data/");
	locationlist.push_back(location1);
	locationlist.push_back(location2);

	server::VirtualServer                 virtual_server;
	server::VirtualServer::ServerNameList server_names;
	server_names.push_back("host2");
	server::VirtualServer::HostPortList host_ports;
	host_ports.push_back(std::make_pair("localhost", 8080));
	server::VirtualServer::ErrorPage error_page(404, "/404.html");

	return new server::VirtualServer(server_names, locationlist, host_ports, 2024, error_page);
}

server::VirtualServer *BuildVirtualServer3() {
	// LocationList
	server::VirtualServer::LocationList locationlist;

	server::VirtualServer                 virtual_server;
	server::VirtualServer::ServerNameList server_names;
	server_names.push_back("host3");
	server::VirtualServer::HostPortList host_ports;
	host_ports.push_back(std::make_pair("127.0.0.10", 8090));
	server::VirtualServer::ErrorPage error_page(404, "/404.html");

	return new server::VirtualServer(server_names, locationlist, host_ports, 1024, error_page);
}

server::VirtualServerAddrList BuildVirtualServerAddrList() {
	server::VirtualServerAddrList virtual_servers;
	server::VirtualServer        *vs1 = BuildVirtualServer1();
	server::VirtualServer        *vs2 = BuildVirtualServer2();
	server::VirtualServer        *vs3 = BuildVirtualServer3();
	virtual_servers.push_back(vs1);
	virtual_servers.push_back(vs2);
	virtual_servers.push_back(vs3);
	return virtual_servers;
}

void DeleteAddrList(server::VirtualServerAddrList &virtual_servers) {
	typedef server::VirtualServerAddrList::const_iterator ItVirtualServer;
	for (ItVirtualServer it = virtual_servers.begin(); it != virtual_servers.end(); ++it) {
		delete *it;
	}
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

/* 以下のテストではメソッドは特に関係ない */
int Test1() {
	// request
	HttpRequestFormat request;
	request.request_line              = RequestLine("GET", "/", "HTTP/1.1");
	request.header_fields[HOST]       = "host1";
	request.header_fields[CONNECTION] = "keep-alive";

	server::VirtualServerAddrList virtual_servers = BuildVirtualServerAddrList();
	CheckServerInfoResult         result   = HttpServerInfoCheck::Check(virtual_servers, request);
	const server::VirtualServer  *vs       = virtual_servers.front();       // host1
	server::Location              location = vs->GetLocationList().front(); // location1

	try {
		COMPARE(result.path, location.request_uri);
		COMPARE(result.index, location.index);
		COMPARE(result.autoindex, location.autoindex);
		COMPARE(result.allowed_methods, location.allowed_methods);
		COMPARE(result.cgi_extension, location.cgi_extension);
		COMPARE(result.upload_directory, location.upload_directory);
		COMPARE(result.redirect.GetValue(), location.redirect);
		COMPARE(result.error_page.GetValue(), virtual_servers.front()->GetErrorPage());
	} catch (const std::exception &e) {
		PrintNg();
		std::cerr << e.what() << '\n';
		DeleteAddrList(virtual_servers);
		return EXIT_FAILURE;
	}
	PrintOk();
	DeleteAddrList(virtual_servers);
	return EXIT_SUCCESS;
}

int Test2() {
	// request
	HttpRequestFormat request;
	request.request_line = RequestLine("GET", "/www/test.html", "HTTP/1.1");
	request.header_fields[HOST] = "host10"; // hostが見つからない場合がデフォルト(host1)に
	request.header_fields[CONNECTION] = "keep-alive";

	server::VirtualServerAddrList virtual_servers = BuildVirtualServerAddrList();
	CheckServerInfoResult         result = HttpServerInfoCheck::Check(virtual_servers, request);
	const server::VirtualServer  *vs     = virtual_servers.front();        // host1
	server::Location location = *(Next(vs->GetLocationList().begin(), 1)); // location2(redirect)

	try {
		COMPARE(result.index, location.index);
		COMPARE(result.autoindex, location.autoindex);
		COMPARE(result.allowed_methods, location.allowed_methods);
		COMPARE(result.cgi_extension, location.cgi_extension);
		COMPARE(result.upload_directory, location.upload_directory);
		COMPARE(result.redirect.GetValue(), location.redirect);
		COMPARE(result.error_page.GetValue(), virtual_servers.front()->GetErrorPage());
	} catch (const std::exception &e) {
		PrintNg();
		std::cerr << e.what() << '\n';
		DeleteAddrList(virtual_servers);
		return EXIT_FAILURE;
	}
	PrintOk();
	DeleteAddrList(virtual_servers);
	return EXIT_SUCCESS;
}

int Test3() {
	// request
	HttpRequestFormat request;
	request.request_line              = RequestLine("GET", "/www/data/test.html", "HTTP/1.1");
	request.header_fields[HOST]       = "host2";
	request.header_fields[CONNECTION] = "keep-alive";

	server::VirtualServerAddrList virtual_servers = BuildVirtualServerAddrList();
	CheckServerInfoResult         result   = HttpServerInfoCheck::Check(virtual_servers, request);
	const server::VirtualServer  *vs       = *(Next(virtual_servers.begin(), 1)); // host2
	server::Location              location = vs->GetLocationList().front(); // location1(alias)

	try {
		COMPARE(result.path, location.alias + "test.html");
		COMPARE(result.index, location.index);
		COMPARE(result.autoindex, location.autoindex);
		COMPARE(result.allowed_methods, location.allowed_methods);
		COMPARE(result.cgi_extension, location.cgi_extension);
		COMPARE(result.upload_directory, location.upload_directory);
		COMPARE(result.redirect.GetValue(), location.redirect);
		COMPARE(result.error_page.GetValue(), virtual_servers.front()->GetErrorPage());
	} catch (const std::exception &e) {
		PrintNg();
		std::cerr << e.what() << '\n';
		DeleteAddrList(virtual_servers);
		return EXIT_FAILURE;
	}
	PrintOk();
	DeleteAddrList(virtual_servers);
	return EXIT_SUCCESS;
}

int Test4() {
	// request
	HttpRequestFormat request;
	request.request_line              = RequestLine("GET", "/web/", "HTTP/1.1");
	request.header_fields[HOST]       = "host2";
	request.header_fields[CONNECTION] = "keep-alive";

	server::VirtualServerAddrList virtual_servers = BuildVirtualServerAddrList();
	CheckServerInfoResult         result = HttpServerInfoCheck::Check(virtual_servers, request);
	const server::VirtualServer  *vs     = *(Next(virtual_servers.begin(), 1)); // host2
	server::Location              location =
		*(Next(vs->GetLocationList().begin(), 1)); // location2(cgi, upload_directory)

	try {
		COMPARE(result.path, location.request_uri);
		COMPARE(result.index, location.index);
		COMPARE(result.autoindex, location.autoindex);
		COMPARE(result.allowed_methods, location.allowed_methods);
		COMPARE(result.cgi_extension, location.cgi_extension);
		COMPARE(result.upload_directory, location.upload_directory);
		COMPARE(result.redirect.GetValue(), location.redirect);
		COMPARE(result.error_page.GetValue(), virtual_servers.front()->GetErrorPage());
	} catch (const std::exception &e) {
		PrintNg();
		std::cerr << e.what() << '\n';
		DeleteAddrList(virtual_servers);
		return EXIT_FAILURE;
	}
	PrintOk();
	DeleteAddrList(virtual_servers);
	return EXIT_SUCCESS;
}

// 仮置き: host3を使ってlocation NOFエラーをチェックしたい
// int Test5() {
// 	// request
// 	const RequestLine request_line = {"GET", "/", "HTTP/1.1"};
// 	HttpRequestFormat request;
// 	request.request_line              = request_line;
// 	request.header_fields[HOST]       = "host3";
// 	request.header_fields[CONNECTION] = "keep-alive";

// 	server::VirtualServerAddrList virtual_servers = BuildVirtualServerAddrList();
// 	CheckServerInfoResult         result = HttpServerInfoCheck::Check(virtual_servers, request);
// 	const server::VirtualServer  *vs     = *(Next(virtual_servers.begin(), 2)); // host3
// 	server::Location              location =
// 		vs->GetLocationList().front(); // location1(nothing)

// 	try {
// 		COMPARE(result.path, location.request_uri);
// 		COMPARE(result.index, location.index);
// 		COMPARE(result.autoindex, location.autoindex);
// 		COMPARE(result.allowed_methods, location.allowed_methods);
// 		COMPARE(result.cgi_extension, location.cgi_extension);
// 		COMPARE(result.upload_directory, location.upload_directory);
// 		COMPARE(result.redirect.GetValue(), location.redirect);
// 		COMPARE(result.error_page.GetValue(), virtual_servers.front()->GetErrorPage());
// 	} catch (const std::exception &e) {
// 		PrintNg();
// 		std::cerr << e.what() << '\n';
// 		DeleteAddrList(virtual_servers);
// 		return EXIT_FAILURE;
// 	}
// 	PrintOk();
// 	DeleteAddrList(virtual_servers);
// 	return EXIT_SUCCESS;
// }

} // namespace

int main() {
	int ret = EXIT_SUCCESS;

	ret |= Test1();
	ret |= Test2();
	ret |= Test3();
	ret |= Test4();
	return ret;
}
