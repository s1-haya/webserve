#include "virtual_server.hpp"
#include "virtual_server_storage.hpp"
#include <cstdlib>

namespace {

typedef server::VirtualServer::LocationList LocationList;

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
	server::VirtualServer vs1(expected_server_name1, expected_locations1);

	std::string  expected_server_name2 = "localhost2";
	LocationList expected_locations2;
	expected_locations2.push_back("/");
	expected_locations2.push_back("/static/");
	server::VirtualServer vs2(expected_server_name2, expected_locations2);

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

	return ret_code;
}

} // namespace

int main() {
	int ret_code = EXIT_SUCCESS;

	ret_code |= RunTestVirtualServerStorage();

	return ret_code;
}
