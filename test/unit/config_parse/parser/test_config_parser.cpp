#include "color.hpp"
#include "context.hpp"
#include "lexer.hpp"
#include "parser.hpp"
/*  消す  */
#include "../../../../srcs/config_parse/context.hpp"
#include "../../../../srcs/config_parse/lexer.hpp"
#include "../../../../srcs/config_parse/parser.hpp"
#include "../../../../srcs/utils/color.hpp"
/*-------*/
#include <cstdlib>
#include <iostream>
#include <sstream>

namespace { /*テスト汎用*/

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

struct Result {
	Result() : is_success(true) {}
	bool        is_success;
	std::string error_log;
};

int GetTestCaseNum() {
	static unsigned int test_case_num = 0;
	++test_case_num;
	return test_case_num;
}

void PrintOk() {
	std::cout << utils::color::GREEN << GetTestCaseNum() << ".[OK] " << utils::color::RESET
			  << std::endl;
}

void PrintNg() {
	std::cerr << utils::color::RED << GetTestCaseNum() << ".[NG] " << utils::color::RESET
			  << std::endl;
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
	return lhs.location == rhs.location && lhs.root == rhs.root && lhs.index == rhs.index &&
		   lhs.allowed_method == rhs.allowed_method;
}

bool operator!=(const LocationCon &lhs, const LocationCon &rhs) {
	return lhs.location != rhs.location || lhs.root != rhs.root || lhs.index != rhs.index ||
		   lhs.allowed_method != rhs.allowed_method;
}

bool operator==(const ServerCon &lhs, const ServerCon &rhs) {
	return lhs.port == rhs.port && lhs.server_name == rhs.server_name &&
		   lhs.location_con == rhs.location_con;
}

bool operator!=(const ServerCon &lhs, const ServerCon &rhs) {
	return lhs.port != rhs.port || lhs.server_name != rhs.server_name ||
		   lhs.location_con != rhs.location_con;
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

std::ostream &operator<<(std::ostream &os, const context::LocationCon location) {
	os << "{location: " << location.location << ", "
	   << "root: " << location.root << ", "
	   << "index: " << location.index << ", "
	   << "allowed_method: " << location.allowed_method << "}";
	return os;
}

std::ostream &operator<<(std::ostream &os, const context::ServerCon server) {
	os << "{port: " << *(server.port.begin()) << ", " // tmp
	   << "server_name: " << server.server_name << ", "
	   << "location_context: " << *(server.location_con.begin()) << "}"; // tmp
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

Result Run(const std::string &src, const ServerList &expected) {
	Result run_result;

	NodeList       nodes;
	lexer::Lexer   lex(src, nodes);
	parser::Parser par(nodes);
	if (par.GetServers() != expected) {
		std::ostringstream error_log;
		error_log << "- Expected: [ " << expected << " ]\n";
		error_log << "- Result  : [ " << par.GetServers() << " ]\n";
		run_result.is_success = false;
		run_result.error_log  = error_log.str();
	}
	return run_result;
}

int Test(const Result &result, const std::string &src) {
	if (result.is_success) {
		PrintOk();
		return EXIT_SUCCESS;
	} else {
		PrintNg();
		PrintError("ConfigParser failed:");
		std::cerr << "src:[\n" << src << "]" << std::endl;
		std::cerr << "--------------------------------" << std::endl;
		std::cerr << result.error_log;
		return EXIT_FAILURE;
	}
}

int RunTests(const TestCase test_cases[], std::size_t num_test_cases) {
	int ret_code = 0;

	for (std::size_t i = 0; i < num_test_cases; i++) {
		const TestCase test_case = test_cases[i];
		ret_code |= Test(Run(test_case.input, test_case.expected), test_case.input);
	}
	return ret_code;
}

} // namespace

int main(int argc, char *argv[]) {
	int ret_code = EXIT_SUCCESS;

	/* Test1 */
	ServerList     expected_result_test_1;
	std::list<int> expected_test_1_ports_1;
	expected_test_1_ports_1.push_back(8080);
	LocationList expected_test_1_locationlist_1;
	// context::LocationCon expected_test_1_location_1 = {"/", "/www/", "index.html", "GET"};
	// expected_test_1_locationlist_1.push_back(expected_test_1_location_1);
	context::ServerCon expected_test_1_server_1 = {
		expected_test_1_ports_1, "localhost", expected_test_1_locationlist_1
	};
	expected_result_test_1.push_back(expected_test_1_server_1);

	static const TestCase test_cases[] = {
		TestCase(
			"server {\n \
				server_name localhost;\n \
				listen 8080;\n \
				}\n",
			expected_result_test_1
		),
	};

	ret_code |= RunTests(test_cases, ARRAY_SIZE(test_cases));

	return ret_code;
}
