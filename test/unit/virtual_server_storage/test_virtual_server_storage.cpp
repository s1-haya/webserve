#include "color.hpp"
#include "virtual_server.hpp"
#include "virtual_server_storage.hpp"
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
// (todo: test_virtual_server.cppと全く同じ関数なのでどうにかしても良いかも)
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

Result
TestIsSameVirtualServer(const server::VirtualServer &vs, const server::VirtualServer &expected_vs) {
	Result result;
	result.is_ok = true;
	std::ostringstream oss;

	// server_name
	const std::string &server_name          = vs.GetServerName();
	const std::string &expected_server_name = expected_vs.GetServerName();
	if (!IsSame(server_name, expected_server_name)) {
		result.is_ok = false;
		oss << "server_name" << std::endl;
		oss << "- result   [" << server_name << "]" << std::endl;
		oss << "- expected [" << expected_server_name << "]" << std::endl;
	}

	// locations
	const LocationList &locations          = vs.GetLocations();
	const LocationList &expected_locations = expected_vs.GetLocations();
	if (!IsSame(locations, expected_locations)) {
		result.is_ok = false;
		oss << "locations" << std::endl;
		oss << "- result  " << std::endl;
		oss << "[" << PrintList(locations) << "]" << std::endl;
		oss << "- expected" << std::endl;
		oss << "[" << PrintList(expected_locations) << "]" << std::endl;
	}

	// ports
	const PortList &ports          = vs.GetPorts();
	const PortList &expected_ports = expected_vs.GetPorts();
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

Result RunGetVirtualServer(
	const server::VirtualServerStorage &vs_storage,
	int                                 server_fd,
	const server::VirtualServer        &expected_vs
) {
	const server::VirtualServer &vs = vs_storage.GetVirtualServer(server_fd);

	return TestIsSameVirtualServer(vs, expected_vs);
}

// -----------------------------------------------------------------------------
// - virtual_serverは以下の想定
// virtual_server | server_name | locations         | ports
// ----------------- ----------------------------------------------
//       vs1      | localhost   | {"/www/"}         | {8080, 12345}
//       vs2      | localhost2  | {"/", "/static/"} | {9999}
int RunTestVirtualServerStorage() {
	int ret_code = EXIT_SUCCESS;

	/* -------------- VirtualServer2個用意 -------------- */
	std::string  expected_server_name1 = "localhost";
	LocationList expected_locations1;
	expected_locations1.push_back("/www/");
	PortList expected_ports1;
	expected_ports1.push_back(8080);
	expected_ports1.push_back(12345);
	server::VirtualServer vs1(expected_server_name1, expected_locations1, expected_ports1);

	std::string  expected_server_name2 = "localhost2";
	LocationList expected_locations2;
	expected_locations2.push_back("/");
	expected_locations2.push_back("/static/");
	PortList expected_ports2;
	expected_ports2.push_back(9999);
	server::VirtualServer vs2(expected_server_name2, expected_locations2, expected_ports2);

	/* -------------- VirtualServerStorage -------------- */
	server::VirtualServerStorage vs_storage;

	// virtual_server2個をvirtual_server_storageに追加
	vs_storage.AddVirtualServer(vs1);
	vs_storage.AddVirtualServer(vs2);

	// - socket通信した結果のserver_fdとvirtual_serverは以下の想定
	// fd | virtual_server  | port
	// ----------------------------
	//  4 |       vs1       | 8080
	//  5 |       vs1       | 12345
	//  6 |       vs2       | 9999

	// server_fdとvirtual_serverの紐づけをvirtual_server_storageに追加
	vs_storage.AddMapping(4, &vs1);
	vs_storage.AddMapping(5, &vs1);
	vs_storage.AddMapping(6, &vs2);

	// getterを使用して期待通りvirtual_serverが追加されてるか・紐づけられているかテスト
	ret_code |= Test(RunGetVirtualServer(vs_storage, 4, vs1));
	ret_code |= Test(RunGetVirtualServer(vs_storage, 5, vs1));
	ret_code |= Test(RunGetVirtualServer(vs_storage, 6, vs2));

	// virtual_server_storageのcopyのテスト
	// copy constructor
	server::VirtualServerStorage copy_vs_storage1(vs_storage);
	ret_code |= Test(RunGetVirtualServer(copy_vs_storage1, 4, vs1));
	ret_code |= Test(RunGetVirtualServer(copy_vs_storage1, 5, vs1));
	ret_code |= Test(RunGetVirtualServer(copy_vs_storage1, 6, vs2));

	// copy assignment operator=
	server::VirtualServerStorage copy_vs_storage2 = vs_storage;
	ret_code |= Test(RunGetVirtualServer(copy_vs_storage2, 4, vs1));
	ret_code |= Test(RunGetVirtualServer(copy_vs_storage2, 5, vs1));
	ret_code |= Test(RunGetVirtualServer(copy_vs_storage2, 6, vs2));

	return ret_code;
}

} // namespace

int main() {
	int ret_code = EXIT_SUCCESS;

	ret_code |= RunTestVirtualServerStorage();

	return ret_code;
}
