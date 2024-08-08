#include "color.hpp"
#include "config.hpp"
#include "context.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

namespace { /*テスト汎用*/

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

struct Result {
	Result() : is_success(true) {}
	bool        is_success;
	std::string error_log;
};

void PrintOk(int test_num) {
	std::cout << utils::color::GREEN << test_num << ".[OK] " << utils::color::RESET << std::endl;
}

void PrintNg(int test_num) {
	std::cerr << utils::color::RED << test_num << ".[NG] " << utils::color::RESET << std::endl;
}

void PrintError(const std::string &message) {
	std::cerr << utils::color::RED << message << utils::color::RESET << std::endl;
}

} // namespace

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

/* namespace内で定義しないと他のコードで使えない */
namespace config {
namespace context {

bool operator==(const LocationCon &lhs, const LocationCon &rhs) {
	return lhs.request_uri == rhs.request_uri && lhs.root == rhs.root && lhs.index == rhs.index &&
		   lhs.autoindex == rhs.autoindex && lhs.allowed_methods == rhs.allowed_methods &&
		   lhs.redirect == rhs.redirect;
}

bool operator!=(const LocationCon &lhs, const LocationCon &rhs) {
	return lhs.request_uri != rhs.request_uri || lhs.root != rhs.root || lhs.index != rhs.index ||
		   lhs.autoindex != rhs.autoindex || lhs.allowed_methods != rhs.allowed_methods ||
		   lhs.redirect != rhs.redirect;
}

bool operator==(const ServerCon &lhs, const ServerCon &rhs) {
	return lhs.port == rhs.port && lhs.server_names == rhs.server_names &&
		   lhs.location_con == rhs.location_con &&
		   lhs.client_max_body_size == rhs.client_max_body_size && lhs.error_page == rhs.error_page;
}

bool operator!=(const ServerCon &lhs, const ServerCon &rhs) {
	return lhs.port != rhs.port || lhs.server_names != rhs.server_names ||
		   lhs.location_con != rhs.location_con ||
		   lhs.client_max_body_size != rhs.client_max_body_size || lhs.error_page != rhs.error_page;
}

} // namespace context
} // namespace config

using namespace config;

namespace { /*parser固有*/

typedef std::list<node::Node>           NodeList;
typedef std::list<context::LocationCon> LocationList;
typedef std::list<context::ServerCon>   ServerList;

// expected
struct TestCase {
	TestCase(const std::string &input, const ServerList &expected)
		: input(input), expected(expected) {}
	const std::string input;
	const ServerList  expected;
};

/* Factory Functions */
// 引数付きのFactory Function: 引数ありコンストラクタはここでしか使わないため
context::LocationCon BuildLocationCon(
	const std::string                          &request_uri,
	const std::string                          &root,
	const std::string                          &index,
	bool                                        autoindex,
	const std::list<std::string>               &allowed_methods,
	const std::pair<unsigned int, std::string> &redirect
) {
	context::LocationCon loc;
	loc.request_uri     = request_uri;
	loc.root            = root;
	loc.index           = index;
	loc.autoindex       = autoindex;
	loc.allowed_methods = allowed_methods;
	loc.redirect        = redirect;
	return loc;
}

// 引数付きのFactory Function: 引数ありコンストラクタはここでしか使わないため
context::ServerCon BuildServerCon(
	const std::list<int>                       &port,
	const std::list<std::string>               &server_names,
	const LocationList                         &location_con,
	std::size_t                                 client_max_body_size,
	const std::pair<unsigned int, std::string> &error_page
) {
	context::ServerCon server;
	server.port                 = port;
	server.server_names         = server_names;
	server.location_con         = location_con;
	server.client_max_body_size = client_max_body_size;
	server.error_page           = error_page;
	return server;
}

/* For server_names, allowed_methods */
std::ostream &operator<<(std::ostream &os, const std::list<std::string> &str_list) {
	std::list<std::string>::const_iterator it = str_list.begin();
	while (it != str_list.end()) {
		os << *it;
		++it;
		if (it != str_list.end()) {
			os << ", ";
		}
	}
	return os;
}

/* For ports */
std::ostream &operator<<(std::ostream &os, const std::list<int> &int_list) {
	std::list<int>::const_iterator it = int_list.begin();
	while (it != int_list.end()) {
		os << *it;
		++it;
		if (it != int_list.end()) {
			os << ", ";
		}
	}
	return os;
}

std::ostream &operator<<(std::ostream &os, const context::LocationCon &location) {
	os << "{location: " << location.request_uri << ", "
	   << "root: " << location.root << ", "
	   << "index: " << location.index << ", "
	   << "autoindex: " << (location.autoindex ? "true" : "false") << ", "
	   << "allowed_method: " << location.allowed_methods << ", "
	   << "return(status): " << location.redirect.first << ", "
	   << "return(index): " << location.redirect.second << "}";
	return os;
}

std::ostream &operator<<(std::ostream &os, const LocationList &locations) {
	LocationList::const_iterator it = locations.begin();
	while (it != locations.end()) {
		os << *it;
		++it;
		if (it != locations.end()) {
			os << ", ";
		}
	}
	return os;
}

std::ostream &operator<<(std::ostream &os, const context::ServerCon &server) {
	os << "{port: " << server.port << ", "
	   << "server_name: " << server.server_names << ", "
	   << "location_context: " << server.location_con << ", "
	   << "client_max_body_size: " << server.client_max_body_size << ", "
	   << "error_page(status): " << server.error_page.first << ", "
	   << "error_page(index): " << server.error_page.second << "}";
	return os;
}

std::ostream &operator<<(std::ostream &os, const ServerList &servers) {
	ServerList::const_iterator it = servers.begin();
	while (it != servers.end()) {
		os << *it;
		++it;
		if (it != servers.end()) {
			os << ", ";
		}
	}
	return os;
}

Result Run(const std::string &file_path, const ServerList &expected) {
	Result run_result;

	ConfigInstance->Create(file_path);
	if (ConfigInstance->servers_ != expected) {
		std::ostringstream error_log;
		error_log << "- Expected: [ " << expected << " ]\n";
		error_log << "- Result  : [ " << ConfigInstance->servers_ << " ]\n";
		run_result.is_success = false;
		run_result.error_log  = error_log.str();
	}
	ConfigInstance->Destroy();
	return run_result;
}

int Test(const Result &result, const std::string &src, int test_num) {
	if (result.is_success) {
		PrintOk(test_num);
		return EXIT_SUCCESS;
	} else {
		PrintNg(test_num);
		PrintError("ConfigParser failed:");
		std::cerr << "src:[\n" << src << "]" << std::endl;
		std::cerr << "--------------------------------" << std::endl;
		std::cerr << result.error_log;
		return EXIT_FAILURE;
	}
}

/* For Error Tests */
int RunErrorTest(
	const std::string &file_path, const ServerList &expected, const std::string &src, int test_num
) {
	int ret_code = EXIT_SUCCESS;

	try {
		Result result = Run(file_path, expected);
		PrintNg(test_num);
		PrintError("ConfigParser failed (No Throw):");
		std::cerr << "src:[\n" << src << "]" << std::endl;
		ret_code |= EXIT_FAILURE;
	} catch (const std::exception &e) {
		PrintOk(test_num);
		utils::Debug(e.what());
	}
	return ret_code;
}

/* Test1 One Server */
ServerList MakeExpectedTest1() {
	ServerList                           expected_result;
	std::list<int>                       expected_ports_1;
	std::list<std::string>               server_names_1;
	LocationList                         expected_locationlist_1;
	std::pair<unsigned int, std::string> error_page_1;
	context::ServerCon                   expected_server_1 = BuildServerCon(
        expected_ports_1, server_names_1, expected_locationlist_1, 1024, error_page_1
    );
	expected_result.push_back(expected_server_1);

	return expected_result;
}

/* Test2 One Server, One Location */
ServerList MakeExpectedTest2() {
	ServerList                           expected_result;
	std::list<int>                       expected_ports_1;
	std::list<std::string>               server_names_1;
	LocationList                         expected_locationlist_1;
	std::list<std::string>               allowed_methods_1;
	std::pair<unsigned int, std::string> redirect_1;
	context::LocationCon                 expected_location_1_1 =
		BuildLocationCon("/", "", "", false, allowed_methods_1, redirect_1);
	expected_locationlist_1.push_back(expected_location_1_1);
	std::pair<unsigned int, std::string> error_page_1;
	context::ServerCon                   expected_server_1 = BuildServerCon(
        expected_ports_1, server_names_1, expected_locationlist_1, 1024, error_page_1
    );
	expected_result.push_back(expected_server_1);

	return expected_result;
}

/* Test3 Server, Location Directive */
ServerList MakeExpectedTest3() {
	ServerList     expected_result;
	std::list<int> expected_ports_1;
	expected_ports_1.push_back(8080);
	std::list<std::string> server_names_1;
	server_names_1.push_back("localhost");
	LocationList                         expected_locationlist_1;
	std::list<std::string>               allowed_methods_1;
	std::pair<unsigned int, std::string> redirect_1;
	context::LocationCon                 expected_location_1_1 =
		BuildLocationCon("/", "/data/", "index.html", false, allowed_methods_1, redirect_1);
	expected_locationlist_1.push_back(expected_location_1_1);
	std::pair<unsigned int, std::string> error_page_1;
	context::ServerCon                   expected_server_1 = BuildServerCon(
        expected_ports_1, server_names_1, expected_locationlist_1, 1024, error_page_1
    );
	expected_result.push_back(expected_server_1);

	return expected_result;
}

/* Test4 Multiple ports */
ServerList MakeExpectedTest4() {
	ServerList             expected_result;
	std::list<std::string> server_names_1;
	server_names_1.push_back("server_name");
	server_names_1.push_back("server");
	std::list<int> expected_ports_1;
	expected_ports_1.push_back(8080);
	expected_ports_1.push_back(8000);
	expected_ports_1.push_back(80);
	LocationList                         expected_locationlist_1;
	std::pair<unsigned int, std::string> error_page_1;
	context::ServerCon                   expected_server_1 = BuildServerCon(
        expected_ports_1, server_names_1, expected_locationlist_1, 1024, error_page_1
    );
	expected_result.push_back(expected_server_1);

	return expected_result;
}

/* Test5 Multiple Locations */
ServerList MakeExpectedTest5() {
	ServerList             expected_result;
	std::list<int>         expected_ports_1;
	std::list<std::string> server_names_1;
	server_names_1.push_back("test.serv");
	LocationList                         expected_locationlist_1;
	std::list<std::string>               allowed_methods_1_1;
	std::pair<unsigned int, std::string> redirect_1_1;
	context::LocationCon                 expected_location_1_1 =
		BuildLocationCon("/", "", "index.html", false, allowed_methods_1_1, redirect_1_1);
	std::list<std::string>               allowed_methods_1_2;
	std::pair<unsigned int, std::string> redirect_1_2;
	context::LocationCon                 expected_location_1_2 =
		BuildLocationCon("/www/", "", "index", false, allowed_methods_1_2, redirect_1_2);
	expected_locationlist_1.push_back(expected_location_1_1);
	expected_locationlist_1.push_back(expected_location_1_2);
	std::pair<unsigned int, std::string> error_page_1;
	context::ServerCon                   expected_server_1 = BuildServerCon(
        expected_ports_1, server_names_1, expected_locationlist_1, 1024, error_page_1
    );
	expected_result.push_back(expected_server_1);

	return expected_result;
}

/* Test6 Tab+Space, Comment */
ServerList MakeExpectedTest6() {
	ServerList             expected_result;
	std::list<std::string> server_names_1;
	server_names_1.push_back("comment.serv");
	std::list<int>                       expected_ports_1;
	LocationList                         expected_locationlist_1;
	std::list<std::string>               allowed_methods_1;
	std::pair<unsigned int, std::string> redirect_1;
	context::LocationCon                 expected_location_1_1 =
		BuildLocationCon("/", "", "index.html", false, allowed_methods_1, redirect_1);
	expected_locationlist_1.push_back(expected_location_1_1);
	std::pair<unsigned int, std::string> error_page_1;
	context::ServerCon                   expected_server_1 = BuildServerCon(
        expected_ports_1, server_names_1, expected_locationlist_1, 1024, error_page_1
    );
	expected_result.push_back(expected_server_1);

	return expected_result;
}

/* Test7 Multiple Servers */
ServerList MakeExpectedTest7() {
	ServerList expected_result;

	std::list<int> expected_ports_1;
	expected_ports_1.push_back(8080);
	std::list<std::string> server_names_1;
	server_names_1.push_back("localhost");
	LocationList                         expected_locationlist_1;
	std::pair<unsigned int, std::string> error_page_1;
	context::ServerCon                   expected_server_1 = BuildServerCon(
        expected_ports_1, server_names_1, expected_locationlist_1, 1024, error_page_1
    );
	expected_result.push_back(expected_server_1);

	std::list<int> expected_ports_2;
	expected_ports_2.push_back(12345);
	std::list<std::string> server_names_2;
	server_names_2.push_back("test.www");
	LocationList                         expected_locationlist_2;
	std::pair<unsigned int, std::string> error_page_2;
	context::ServerCon                   expected_server_2 = BuildServerCon(
        expected_ports_2, server_names_2, expected_locationlist_2, 1024, error_page_2
    );
	expected_result.push_back(expected_server_2);

	return expected_result;
}

/* Test8 Additional Directives */
ServerList MakeExpectedTest8() {
	ServerList     expected_result;
	std::list<int> expected_ports_1;
	expected_ports_1.push_back(8080);
	std::list<std::string> server_names_1;
	server_names_1.push_back("localhost");
	LocationList           expected_locationlist_1;
	std::list<std::string> allowed_methods_1;
	allowed_methods_1.push_back("GET");
	allowed_methods_1.push_back("POST");
	std::pair<unsigned int, std::string> redirect_1(302, "/redirect.html");
	context::LocationCon                 expected_location_1_1 =
		BuildLocationCon("/", "/data/", "index.html", true, allowed_methods_1, redirect_1);
	expected_locationlist_1.push_back(expected_location_1_1);
	std::pair<unsigned int, std::string> error_page_1(404, "/404.html");
	context::ServerCon                   expected_server_1 = BuildServerCon(
        expected_ports_1, server_names_1, expected_locationlist_1, 2024, error_page_1
    );
	expected_result.push_back(expected_server_1);

	return expected_result;
}

} // namespace

int main(int argc, char *argv[]) {
	(void)argc;
	int ret_code = EXIT_SUCCESS;

	std::ifstream conf_file(argv[2]);
	if (!conf_file) {
		return EXIT_FAILURE;
	}
	std::stringstream buffer;
	buffer << conf_file.rdbuf();

	ServerList expected;
	int        test_num = std::atoi(argv[3]);
	if (std::string(argv[1]) == "success") {
		switch (test_num) {
		case 1:
			expected = MakeExpectedTest1();
			break;
		case 2:
			expected = MakeExpectedTest2();
			break;
		case 3:
			expected = MakeExpectedTest3();
			break;
		case 4:
			expected = MakeExpectedTest4();
			break;
		case 5:
			expected = MakeExpectedTest5();
			break;
		case 6:
			expected = MakeExpectedTest6();
			break;
		case 7:
			expected = MakeExpectedTest7();
			break;
		case 8:
			expected = MakeExpectedTest8();
			break;
		default:
			break;
		}
		ret_code |= Test(Run(argv[2], expected), buffer.str(), test_num);
	} else if (std::string(argv[1]) == "error") {
		ret_code |= RunErrorTest(argv[2], expected, buffer.str(), test_num);
	}

	return ret_code;
}
