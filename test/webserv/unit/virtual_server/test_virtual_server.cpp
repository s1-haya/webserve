#include "color.hpp"
#include "virtual_server.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream> // ostringstream

namespace server {

// テストfail時のServerNameList,AllowedMethodListのdebug出力用
std::ostream &operator<<(std::ostream &os, const std::list<std::string> &list) {
	typedef std::list<std::string>::const_iterator It;
	for (It it = list.begin(); it != list.end(); ++it) {
		os << "[" << *it << "]";
	}
	os << std::endl;
	return os;
}

// テストfail時のLocationListのdebug出力用
std::ostream &operator<<(std::ostream &os, const VirtualServer::LocationList &locations) {
	typedef VirtualServer::LocationList::const_iterator It;
	for (It it = locations.begin(); it != locations.end(); ++it) {
		const Location &location = *it;
		os << "  request_uri: [" << location.request_uri << "]" << std::endl;
		os << "  alias: [" << location.alias << "]" << std::endl;
		os << "  index: [" << location.index << "]" << std::endl;
		os << "  autoindex: [" << (location.autoindex ? "on" : "off") << "]" << std::endl;
		os << "  allowed_methods: " << location.allowed_methods << std::endl;
		os << "  redirect: [" << location.request_uri << "]" << std::endl;
		os << "  cgi_extension: [" << location.cgi_extension << "]" << std::endl;
		os << "  upload_directory: [" << location.upload_directory << "]" << std::endl;
		if (++It(it) != locations.end()) {
			os << std::endl;
		}
	}
	return os;
}

// std::list<Location>のoperator==が呼ばれる時用にstruct Locationのoperator==の実装
bool operator==(const Location &lhs, const Location &rhs) {
	return lhs.request_uri == rhs.request_uri && lhs.alias == rhs.alias && lhs.index == rhs.index &&
		   lhs.autoindex == rhs.autoindex && lhs.allowed_methods == rhs.allowed_methods &&
		   lhs.redirect == rhs.redirect && lhs.cgi_extension == rhs.cgi_extension &&
		   lhs.upload_directory == rhs.upload_directory;
}

// テストfail時のHostPortListのdebug出力用
std::ostream &operator<<(std::ostream &os, const VirtualServer::HostPortList &host_ports) {
	typedef VirtualServer::HostPortList::const_iterator It;
	for (It it = host_ports.begin(); it != host_ports.end(); ++it) {
		const VirtualServer::HostPortPair &host_port_pair = *it;
		os << "host: [" << host_port_pair.first << "], port: [" << host_port_pair.second << "]";
		if (++It(it) != host_ports.end()) {
			os << std::endl;
		}
	}
	return os;
}

// テストfail時のErrorPageのdebug出力用
std::ostream &operator<<(std::ostream &os, const VirtualServer::ErrorPage &error_page) {
	os << "[" << error_page.first << "][" << error_page.second << "]";
	return os;
}

} // namespace server

namespace {

typedef server::Location                    Location;
typedef server::Location::AllowedMethodList AllowedMethodList;
typedef server::Location::Redirect          Redirect;

typedef server::VirtualServer::ServerNameList ServerNameList;
typedef server::VirtualServer::LocationList   LocationList;
typedef server::VirtualServer::HostPortPair   HostPortPair;
typedef server::VirtualServer::HostPortList   HostPortList;
typedef server::VirtualServer::ErrorPage      ErrorPage;

struct Result {
	bool        is_ok;
	std::string error_log;
};

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

void PrintError(const std::string &message) {
	std::cerr << utils::color::RED << message << utils::color::RESET << std::endl;
}

template <typename T>
bool IsSame(const T &result, const T &expected) {
	return result == expected;
}

int Test(Result result) {
	if (result.is_ok) {
		PrintOk();
		return EXIT_SUCCESS;
	}
	PrintNg();
	PrintError(result.error_log);
	return EXIT_FAILURE;
}

// -----------------------------------------------------------------------------
Result TestIsSameMembers(
	const server::VirtualServer &under_test_vs,
	const ServerNameList        &expected_server_names,
	const LocationList          &expected_locations,
	const HostPortList          &expected_host_ports,
	std::size_t                  expected_client_max_body_size,
	const ErrorPage             &expected_error_page

) {
	using namespace server;

	Result result;
	result.is_ok = true;
	std::ostringstream oss;

	// VirtualServer.GetServerNameList()
	const ServerNameList &server_names = under_test_vs.GetServerNameList();
	if (!IsSame(server_names, expected_server_names)) {
		result.is_ok = false;
		oss << "server_names" << std::endl;
		oss << "- result  " << std::endl;
		oss << server_names << std::endl;
		oss << "- expected" << std::endl;
		oss << expected_server_names << std::endl;
	}

	// VirtualServer.GetLocationList()
	const LocationList &locations = under_test_vs.GetLocationList();
	if (!IsSame(locations, expected_locations)) {
		result.is_ok = false;
		oss << "locations" << std::endl;
		oss << "- result  " << std::endl;
		oss << locations << std::endl;
		oss << "- expected" << std::endl;
		oss << expected_locations << std::endl;
	}

	// VirtualServer.GetHostPortList()
	const HostPortList &host_ports = under_test_vs.GetHostPortList();
	if (!IsSame(host_ports, expected_host_ports)) {
		result.is_ok = false;
		oss << "host_ports" << std::endl;
		oss << "- result  " << std::endl;
		oss << host_ports << std::endl;
		oss << "- expected" << std::endl;
		oss << expected_host_ports << std::endl;
	}

	// VirtualServer.GetClientMaxBodySize()
	const std::size_t &client_max_body_size = under_test_vs.GetClientMaxBodySize();
	if (!IsSame(client_max_body_size, expected_client_max_body_size)) {
		result.is_ok = false;
		oss << "client_max_body_size" << std::endl;
		oss << "- result   [" << client_max_body_size << "]" << std::endl;
		oss << "- expected [" << expected_client_max_body_size << "]" << std::endl;
	}

	// VirtualServer.GetErrorPage()
	const ErrorPage &error_page = under_test_vs.GetErrorPage();
	if (!IsSame(error_page, expected_error_page)) {
		result.is_ok = false;
		oss << "error_page" << std::endl;
		oss << "- result  : " << error_page << std::endl;
		oss << "- expected: " << expected_error_page << std::endl;
	}

	result.error_log = oss.str();
	return result;
}

// test default constructor
Result RunDefaultConstructor(
	const ServerNameList &expected_server_names,
	const LocationList   &expected_locations,
	const HostPortList   &expected_host_ports,
	std::size_t           expected_client_max_body_size,
	const ErrorPage      &expected_error_page
) {
	// default constructor
	server::VirtualServer virtual_server;

	return TestIsSameMembers(
		virtual_server,
		expected_server_names,
		expected_locations,
		expected_host_ports,
		expected_client_max_body_size,
		expected_error_page
	);
}

// test copy constructor
Result RunCopyConstructor(
	const ServerNameList &expected_server_names,
	const LocationList   &expected_locations,
	const HostPortList   &expected_host_ports,
	std::size_t           expected_client_max_body_size,
	const ErrorPage      &expected_error_page
) {
	// construct
	server::VirtualServer virtual_server(
		expected_server_names,
		expected_locations,
		expected_host_ports,
		expected_client_max_body_size,
		expected_error_page
	);

	// copy
	server::VirtualServer copy_virtual_server(virtual_server);

	return TestIsSameMembers(
		copy_virtual_server,
		expected_server_names,
		expected_locations,
		expected_host_ports,
		expected_client_max_body_size,
		expected_error_page
	);
}

// test copy assignment operator=
Result RunCopyAssignmentOperator(
	const ServerNameList &expected_server_names,
	const LocationList   &expected_locations,
	const HostPortList   &expected_host_ports,
	std::size_t           expected_client_max_body_size,
	const ErrorPage      &expected_error_page
) {
	// construct
	server::VirtualServer virtual_server(
		expected_server_names,
		expected_locations,
		expected_host_ports,
		expected_client_max_body_size,
		expected_error_page
	);

	// copy assignment operator=
	server::VirtualServer copy_virtual_server = virtual_server;
	return TestIsSameMembers(
		copy_virtual_server,
		expected_server_names,
		expected_locations,
		expected_host_ports,
		expected_client_max_body_size,
		expected_error_page
	);
}

Location CreateLocation(
	const std::string       &request_uri,
	const std::string       &alias,
	const std::string       &index,
	bool                     autoindex,
	const AllowedMethodList &allowed_methods,
	const Redirect          &redirect,
	const std::string       &cgi_extension,
	const std::string       &upload_directory

) {
	server::Location location_directive;
	location_directive.request_uri      = request_uri;
	location_directive.alias            = alias;
	location_directive.index            = index;
	location_directive.autoindex        = autoindex;
	location_directive.allowed_methods  = allowed_methods;
	location_directive.redirect         = redirect;
	location_directive.cgi_extension    = cgi_extension;
	location_directive.upload_directory = upload_directory;
	return location_directive;
}

} // namespace

int main() {
	int ret_code = EXIT_SUCCESS;

	/* -------------- expected用意 -------------- */
	// server_name : 0個
	// location    : 0個
	// host:port   : 0個
	const ServerNameList expected_server_names1;
	const LocationList   expected_locations1;
	const HostPortList   expected_host_ports1;
	const std::size_t    expected_client_max_body_size1 = 1024;
	const ErrorPage      error_page1                    = std::make_pair(0, "");

	// server_name : 1個
	// location    : 1個
	// host:port   : 1個
	ServerNameList expected_server_names2;
	expected_server_names2.push_back("server_name1");
	AllowedMethodList allowed_methods2;
	allowed_methods2.push_back("GET");
	LocationList expected_locations2;
	expected_locations2.push_back(CreateLocation(
		"/www/",
		"/data/",
		"index.html",
		true,
		allowed_methods2,
		std::make_pair(400, "/redirect"),
		".py",
		"/upload"
	));
	HostPortList expected_host_ports2;
	expected_host_ports2.push_back(std::make_pair("host1", 9999));
	const std::size_t expected_client_max_body_size2 = 2048;
	const ErrorPage   error_page2                    = std::make_pair(404, "/error_page.html");

	// server_name : 複数
	// location    : 複数
	// host:port   : 複数
	ServerNameList expected_server_names3;
	expected_server_names3.push_back("server_name1");
	expected_server_names3.push_back("server_name2");
	AllowedMethodList allowed_methods3;
	allowed_methods3.push_back("GET");
	allowed_methods3.push_back("POST");
	LocationList expected_locations3;
	expected_locations3.push_back(CreateLocation(
		"/",
		"/data/www/test",
		"index.html",
		false,
		allowed_methods2,
		std::make_pair(302, "/redirect.html"),
		".php",
		"/upload"
	));
	allowed_methods3.push_back("DELETE");
	expected_locations3.push_back(CreateLocation(
		"/static/",
		"/data/www",
		"index.html",
		true,
		allowed_methods3,
		std::make_pair(404, "/redirect.html"),
		".perl",
		"/upload"
	));
	HostPortList expected_host_ports3;
	expected_host_ports3.push_back(std::make_pair("host1", 8080));
	expected_host_ports3.push_back(std::make_pair("host2", 12345));
	const std::size_t expected_client_max_body_size3 = 4096;
	const ErrorPage   error_page3                    = std::make_pair(404, "/error.html");

	/* ------------------ test ------------------ */
	ret_code |= Test(RunDefaultConstructor(
		expected_server_names1,
		expected_locations1,
		expected_host_ports1,
		expected_client_max_body_size1,
		error_page1
	));
	ret_code |= Test(RunCopyConstructor(
		expected_server_names2,
		expected_locations2,
		expected_host_ports2,
		expected_client_max_body_size2,
		error_page2
	));
	ret_code |= Test(RunCopyAssignmentOperator(
		expected_server_names3,
		expected_locations3,
		expected_host_ports3,
		expected_client_max_body_size3,
		error_page3
	));

	return ret_code;
}
