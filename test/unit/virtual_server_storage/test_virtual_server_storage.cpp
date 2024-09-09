#include "color.hpp"
#include "virtual_server.hpp"
#include "virtual_server_storage.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream> // ostringstream

namespace server {

// テストfail時のServerNameListのdebug出力用
std::ostream &operator<<(std::ostream &os, const VirtualServer::ServerNameList &list) {
	typedef VirtualServer::ServerNameList::const_iterator It;
	for (It it = list.begin(); it != list.end(); ++it) {
		os << "[" << *it << "]";
	}
	os << std::endl;
	return os;
}

} // namespace server

namespace {

typedef server::Location                    Location;
typedef server::Location::AllowedMethodList AllowedMethodList;

typedef server::VirtualServer::ServerNameList ServerNameList;
typedef server::VirtualServer::LocationList   LocationList;
typedef server::VirtualServer::HostPortPair   HostPortPair;
typedef server::VirtualServer::HostPortList   HostPortList;
typedef server::VirtualServer::ErrorPage      ErrorPage;

typedef server::VirtualServerStorage::VirtualServerAddrList VirtualServerAddrList;

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
Result TestIsSameVirtualServer(
	const VirtualServerAddrList &vs_addr_list, const VirtualServerAddrList &expected_vs_addr_list
) {
	using namespace server;

	Result result;
	result.is_ok = true;

	if (vs_addr_list.size() != expected_vs_addr_list.size()) {
		result.is_ok     = false;
		result.error_log = "wrong virtual_server_addr_list's size()";
		return result;
	}

	std::ostringstream oss;

	typedef VirtualServerAddrList::const_iterator Itr;
	Itr                                           it_vs          = vs_addr_list.begin();
	Itr                                           it_expected_vs = expected_vs_addr_list.begin();
	for (; it_vs != vs_addr_list.end(); ++it_vs, ++it_expected_vs) {
		const VirtualServer *vs          = *it_vs;
		const VirtualServer *expected_vs = *it_expected_vs;

		// virtual_server_addr, server_name
		if (!IsSame(vs, expected_vs)) {
			result.is_ok = false;
			oss << "virtual_server_addr" << std::endl;
			oss << "- result   [" << vs << "]," << vs->GetServerNameList() << std::endl;
			oss << "- expected [" << expected_vs << "]," << expected_vs->GetServerNameList()
				<< std::endl;
		}
	}
	result.error_log = oss.str();
	return result;
}

Result RunGetVirtualServer(
	const server::VirtualServerStorage &vs_storage,
	const HostPortPair                 &host_port,
	const VirtualServerAddrList        &expected_vs_addr_list
) {
	const VirtualServerAddrList &vs_addr_list = vs_storage.GetVirtualServerAddrList(host_port);

	return TestIsSameVirtualServer(vs_addr_list, expected_vs_addr_list);
}

server::VirtualServer
CreateVirtualServer(const ServerNameList &server_names, const HostPortList &host_ports) {
	const LocationList locations;
	const std::size_t  client_max_body_size = 1024;
	const ErrorPage    error_page           = std::make_pair(404, "/error_page.html");

	return server::VirtualServer(
		server_names, locations, host_ports, client_max_body_size, error_page
	);
}

// -----------------------------------------------------------------------------
// - virtual_serverは以下の想定
// virtual_server | server_name          | host:port
// -----------------------------------------------------------------------------
//       vs1      | localhost            | {host1:8080, host2:12345}
//       vs2      | localhost2,test_serv | {host1:8080, host3:9999}
int RunTestVirtualServerStorage() {
	int ret_code = EXIT_SUCCESS;

	/* -------------- HostPortPair3個用意 -------------- */
	HostPortPair host_port1 = std::make_pair("host1", 8080);
	HostPortPair host_port2 = std::make_pair("host2", 12345);
	HostPortPair host_port3 = std::make_pair("host3", 9999);

	/* -------------- VirtualServer2個用意 -------------- */
	ServerNameList server_name1;
	server_name1.push_back("localhost");
	HostPortList host_ports;
	host_ports.push_back(host_port1);
	host_ports.push_back(host_port2);
	const server::VirtualServer vs1 = CreateVirtualServer(server_name1, host_ports);

	ServerNameList server_name2;
	server_name2.push_back("localhost2");
	server_name2.push_back("test_serv");
	HostPortList host_ports2;
	host_ports2.push_back(host_port1);
	host_ports2.push_back(host_port3);
	const server::VirtualServer vs2 = CreateVirtualServer(server_name2, host_ports2);

	/* -------------- VirtualServerAddrList用意 -------------- */
	VirtualServerAddrList expected_vs_addr_list1;
	expected_vs_addr_list1.push_back(&vs1);
	expected_vs_addr_list1.push_back(&vs2);

	VirtualServerAddrList expected_vs_addr_list2;
	expected_vs_addr_list2.push_back(&vs1);

	VirtualServerAddrList expected_vs_addr_list3;
	expected_vs_addr_list3.push_back(&vs2);

	/* -------------- VirtualServerStorage -------------- */
	server::VirtualServerStorage vs_storage;

	// virtual_server2個をvirtual_server_storageに追加
	vs_storage.AddVirtualServer(vs1);
	vs_storage.AddVirtualServer(vs2);

	// - socket通信した結果のserver_fdとvirtual_serverは以下の想定
	// fd | virtual_server  | host:port
	// ----------------------------
	//  4 |     vs1, vs2    | host1:8080
	//  5 |       vs1       | host2:12345
	//  6 |       vs2       | host3:9999

	// server_fdとvirtual_serverの紐づけをvirtual_server_storageに追加
	vs_storage.AddMapping(host_port1, &vs1);
	vs_storage.AddMapping(host_port1, &vs2);
	vs_storage.AddMapping(host_port2, &vs1);
	vs_storage.AddMapping(host_port3, &vs2);

	// getterを使用して期待通りvirtual_serverが追加されてるか・紐づけられているかテスト
	ret_code |= Test(RunGetVirtualServer(vs_storage, host_port1, expected_vs_addr_list1));
	ret_code |= Test(RunGetVirtualServer(vs_storage, host_port2, expected_vs_addr_list2));
	ret_code |= Test(RunGetVirtualServer(vs_storage, host_port3, expected_vs_addr_list3));

	// virtual_server_storageのcopyのテスト
	// copy constructor
	server::VirtualServerStorage copy_vs_storage1(vs_storage);
	ret_code |= Test(RunGetVirtualServer(copy_vs_storage1, host_port1, expected_vs_addr_list1));
	ret_code |= Test(RunGetVirtualServer(copy_vs_storage1, host_port2, expected_vs_addr_list2));
	ret_code |= Test(RunGetVirtualServer(copy_vs_storage1, host_port3, expected_vs_addr_list3));

	// copy assignment operator=
	server::VirtualServerStorage copy_vs_storage2 = vs_storage;
	ret_code |= Test(RunGetVirtualServer(copy_vs_storage2, host_port1, expected_vs_addr_list1));
	ret_code |= Test(RunGetVirtualServer(copy_vs_storage2, host_port2, expected_vs_addr_list2));
	ret_code |= Test(RunGetVirtualServer(copy_vs_storage2, host_port3, expected_vs_addr_list3));

	return ret_code;
}

} // namespace

int main() {
	int ret_code = EXIT_SUCCESS;

	ret_code |= RunTestVirtualServerStorage();

	return ret_code;
}
