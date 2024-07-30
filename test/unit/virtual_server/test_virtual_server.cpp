#include "color.hpp"
#include "virtual_server.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream> // ostringstream

namespace {

typedef server::VirtualServer::LocationList LocationList;
typedef server::VirtualServer::PortList     PortList;

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
// テストfail時のlist(locations,ports)のdebug出力
template <typename T>
std::string PrintList(const T &list) {
	std::ostringstream oss;

	typedef typename T::const_iterator It;
	for (It it = list.begin(); it != list.end(); ++it) {
		oss << *it;
		if (++It(it) != list.end()) {
			oss << std::endl;
		}
	}
	return oss.str();
}

Result TestIsSameMembers(
	const server::VirtualServer &under_test_vs,
	const std::string           &expected_server_name,
	const LocationList          &expected_locations,
	const PortList              &expected_ports
) {
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
		oss << "[" << PrintList(locations) << "]" << std::endl;
		oss << "- expected" << std::endl;
		oss << "[" << PrintList(expected_locations) << "]" << std::endl;
	}

	// VirtualServer.GetPorts()
	const PortList &ports = under_test_vs.GetPorts();
	if (!IsSame(ports, expected_ports)) {
		result.is_ok = false;
		oss << "ports" << std::endl;
		oss << "- result  " << std::endl;
		oss << "[" << PrintList(ports) << "]" << std::endl;
		oss << "- expected" << std::endl;
		oss << "[" << PrintList(expected_ports) << "]" << std::endl;
	}

	result.error_log = oss.str();
	return result;
}

// test default constructor
Result RunDefaultConstructor(
	const std::string  &expected_server_name,
	const LocationList &expected_locations,
	const PortList     &expected_ports
) {
	// default constructor
	server::VirtualServer virtual_server;

	return TestIsSameMembers(
		virtual_server, expected_server_name, expected_locations, expected_ports
	);
}

// test copy constructor
Result RunCopyConstructor(
	const std::string  &expected_server_name,
	const LocationList &expected_locations,
	const PortList     &expected_ports
) {
	// construct
	server::VirtualServer virtual_server(expected_server_name, expected_locations, expected_ports);

	// copy
	server::VirtualServer copy_virtual_server(virtual_server);

	return TestIsSameMembers(
		copy_virtual_server, expected_server_name, expected_locations, expected_ports
	);
}

// test copy assignment operator=
Result RunCopyAssignmentOperator(
	const std::string  &expected_server_name,
	const LocationList &expected_locations,
	const PortList     &expected_ports
) {
	// construct
	server::VirtualServer virtual_server(expected_server_name, expected_locations, expected_ports);

	// copy assignment operator=
	server::VirtualServer copy_virtual_server = virtual_server;
	return TestIsSameMembers(
		copy_virtual_server, expected_server_name, expected_locations, expected_ports
	);
}

} // namespace

int main() {
	int ret_code = EXIT_SUCCESS;

	/* -------------- expected用意 -------------- */
	// location: 0個
	// port    : 0個
	LocationList expected_locations1;
	PortList     expected_ports1;

	// location: 1個
	// port    : 1個
	LocationList expected_locations2;
	expected_locations2.push_back("/");
	PortList expected_ports2;
	expected_ports2.push_back(9999);

	// location: 複数
	// port    : 複数
	LocationList expected_locations3;
	expected_locations3.push_back("/static/");
	expected_locations3.push_back("/images/");
	PortList expected_ports3;
	expected_ports3.push_back(8080);
	expected_ports3.push_back(12345);

	/* ------------------ test ------------------ */
	ret_code |= Test(RunDefaultConstructor("", expected_locations1, expected_ports1));
	ret_code |= Test(RunCopyConstructor("localhost", expected_locations2, expected_ports2));
	ret_code |= Test(RunCopyAssignmentOperator("localhost2", expected_locations3, expected_ports3));

	return ret_code;
}
