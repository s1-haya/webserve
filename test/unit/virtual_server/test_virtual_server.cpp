#include "color.hpp"
#include "virtual_server.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream> // ostringstream

namespace {

typedef server::VirtualServer::LocationList LocationList;

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
// テストfail時のlocationsのdebug出力
std::string PrintLocations(const LocationList &locations) {
	std::ostringstream oss;

	typedef LocationList::const_iterator It;
	for (It it = locations.begin(); it != locations.end(); ++it) {
		oss << *it;
		if (++LocationList::const_iterator(it) != locations.end()) {
			oss << std::endl;
		}
	}
	return oss.str();
}

Result TestIsSameMembers(
	const server::VirtualServer &a,
	const std::string           &expected_server_name,
	const std::string           &expected_location
) {
	Result result;
	result.is_ok = true;
	std::ostringstream oss;

	// VirtualServer.GetServerName()
	const std::string &server_name = a.GetServerName();
	if (!IsSame(server_name, expected_server_name)) {
		result.is_ok = false;
		oss << "server_name" << std::endl;
		oss << "- result   [" << server_name << "]" << std::endl;
		oss << "- expected [" << expected_server_name << "]" << std::endl;
	}

	// VirtualServer.GetLocation()
	const std::string &location = a.GetLocation();
	if (!IsSame(location, expected_location)) {
		result.is_ok = false;
		oss << "location" << std::endl;
		oss << "- result   [" << location << "]" << std::endl;
		oss << "- expected [" << expected_location << "]" << std::endl;
	}

	result.error_log = oss.str();
	return result;
}

// test default constructor
Result RunDefaultConstructor(
	const std::string &expected_server_name, const std::string &expected_location
) {
	// default constructor
	server::VirtualServer virtual_server;

	return TestIsSameMembers(virtual_server, expected_server_name, expected_location);
}

// test copy constructor
Result
RunCopyConstructor(const std::string &expected_server_name, const std::string &expected_location) {
	// construct
	server::VirtualServer virtual_server(expected_server_name, expected_location);

	// copy
	server::VirtualServer copy_virtual_server(virtual_server);

	return TestIsSameMembers(copy_virtual_server, expected_server_name, expected_location);
}

// test copy assignment operator=
Result RunCopyAssignmentOperator(
	const std::string &expected_server_name, const std::string &expected_location
) {
	// construct
	server::VirtualServer virtual_server(expected_server_name, expected_location);

	// copy assignment operator=
	server::VirtualServer copy_virtual_server = virtual_server;
	return TestIsSameMembers(copy_virtual_server, expected_server_name, expected_location);
}

} // namespace

int main() {
	int ret_code = EXIT_SUCCESS;

	ret_code |= Test(RunDefaultConstructor("", ""));
	ret_code |= Test(RunCopyConstructor("localhost", "/"));
	ret_code |= Test(RunCopyAssignmentOperator("localhost", "/www/"));

	return ret_code;
}
