#include "color.hpp"
#include "virtual_server.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream> // ostringstream

namespace server {

// テストfail時のLocationListのdebug出力用
std::ostream &operator<<(std::ostream &os, const VirtualServer::LocationList &locations) {
	typedef VirtualServer::LocationList::const_iterator It;
	for (It it = locations.begin(); it != locations.end(); ++it) {
		const Location &location = *it;
		os << "location: [" << location.location << "], root: [" << location.root << "], index: ["
		   << location.index << "]";
		if (++It(it) != locations.end()) {
			os << std::endl;
		}
	}
	return os;
}

// std::list<Location>のoperator==が呼ばれる時用にstruct Locationのoperator==の実装
bool operator==(const Location &lhs, const Location &rhs) {
	return lhs.location == rhs.location && lhs.root == rhs.root && lhs.index == rhs.index &&
		   lhs.allowed_method == rhs.allowed_method;
}

// テストfail時のHostPortListのdebug出力用
std::ostream &operator<<(std::ostream &os, const VirtualServer::HostPortList &host_port_list) {
	typedef VirtualServer::HostPortList::const_iterator It;
	for (It it = host_port_list.begin(); it != host_port_list.end(); ++it) {
		const VirtualServer::HostPortPair &host_port_pair = *it;
		os << "host: [" << host_port_pair.first << "], port: [" << host_port_pair.second << "]";
		if (++It(it) != host_port_list.end()) {
			os << std::endl;
		}
	}
	return os;
}

} // namespace server

namespace {

typedef server::Location                    Location;
typedef server::VirtualServer::LocationList LocationList;
typedef server::VirtualServer::HostPortPair HostPortPair;
typedef server::VirtualServer::HostPortList HostPortList;

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
	const std::string           &expected_server_name,
	const LocationList          &expected_locations,
	const HostPortList          &expected_host_port_list
) {
	using namespace server;

	Result result;
	result.is_ok = true;
	std::ostringstream oss;

	// VirtualServer.GetServerName()
	const std::string &server_name = under_test_vs.GetServerName();
	if (!IsSame(server_name, expected_server_name)) {
		result.is_ok = false;
		oss << "server_name" << std::endl;
		oss << "- result   [" << server_name << "]" << std::endl;
		oss << "- expected [" << expected_server_name << "]" << std::endl;
	}

	// VirtualServer.GetLocations()
	const LocationList &locations = under_test_vs.GetLocations();
	if (!IsSame(locations, expected_locations)) {
		result.is_ok = false;
		oss << "locations" << std::endl;
		oss << "- result  " << std::endl;
		oss << locations << std::endl;
		oss << "- expected" << std::endl;
		oss << expected_locations << std::endl;
	}

	// VirtualServer.GetHostPortList()
	const HostPortList &host_port_list = under_test_vs.GetHostPortList();
	if (!IsSame(host_port_list, expected_host_port_list)) {
		result.is_ok = false;
		oss << "host_port_list" << std::endl;
		oss << "- result  " << std::endl;
		oss << host_port_list << std::endl;
		oss << "- expected" << std::endl;
		oss << expected_host_port_list << std::endl;
	}

	result.error_log = oss.str();
	return result;
}

// test default constructor
Result RunDefaultConstructor(
	const std::string  &expected_server_name,
	const LocationList &expected_locations,
	const HostPortList &expected_host_port_list
) {
	// default constructor
	server::VirtualServer virtual_server;

	return TestIsSameMembers(
		virtual_server, expected_server_name, expected_locations, expected_host_port_list
	);
}

// test copy constructor
Result RunCopyConstructor(
	const std::string  &expected_server_name,
	const LocationList &expected_locations,
	const HostPortList &expected_host_port_list
) {
	// construct
	server::VirtualServer virtual_server(
		expected_server_name, expected_locations, expected_host_port_list
	);

	// copy
	server::VirtualServer copy_virtual_server(virtual_server);

	return TestIsSameMembers(
		copy_virtual_server, expected_server_name, expected_locations, expected_host_port_list
	);
}

// test copy assignment operator=
Result RunCopyAssignmentOperator(
	const std::string  &expected_server_name,
	const LocationList &expected_locations,
	const HostPortList &expected_host_port_list
) {
	// construct
	server::VirtualServer virtual_server(
		expected_server_name, expected_locations, expected_host_port_list
	);

	// copy assignment operator=
	server::VirtualServer copy_virtual_server = virtual_server;
	return TestIsSameMembers(
		copy_virtual_server, expected_server_name, expected_locations, expected_host_port_list
	);
}

Location CreateLocation(
	const std::string &location,
	const std::string &root,
	const std::string &index,
	const std::string &allowed_method
) {
	server::Location location_directive;
	location_directive.location       = location;
	location_directive.root           = root;
	location_directive.index          = index;
	location_directive.allowed_method = allowed_method;
	return location_directive;
}

} // namespace

int main() {
	int ret_code = EXIT_SUCCESS;

	/* -------------- expected用意 -------------- */
	// location : 0個
	// host:port: 0個
	LocationList expected_locations1;
	HostPortList expected_host_port_list1;

	// location : 1個
	// host:port: 1個
	LocationList expected_locations2;
	expected_locations2.push_back(CreateLocation("/www/", "/data/", "index.html", "GET"));
	HostPortList expected_host_port_list2;
	expected_host_port_list2.push_back(std::make_pair("host1", 9999));

	// location : 複数
	// host:port: 複数
	LocationList expected_locations3;
	expected_locations3.push_back(CreateLocation("/", "/data/www/test", "index.html", "GET POST"));
	expected_locations3.push_back(
		CreateLocation("/static/", "/data/www", "index.html", "GET POST DELETE")
	);
	HostPortList expected_host_port_list3;
	expected_host_port_list3.push_back(std::make_pair("host1", 8080));
	expected_host_port_list3.push_back(std::make_pair("host2", 12345));

	/* ------------------ test ------------------ */
	ret_code |= Test(RunDefaultConstructor("", expected_locations1, expected_host_port_list1));
	ret_code |=
		Test(RunCopyConstructor("localhost", expected_locations2, expected_host_port_list2));
	ret_code |=
		Test(RunCopyAssignmentOperator("localhost2", expected_locations3, expected_host_port_list3)
		);

	return ret_code;
}
