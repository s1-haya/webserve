#include "color.hpp"
#include "lexer.hpp"
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

int Test(Result result) {
	if (result.is_success) {
		PrintOk();
		return EXIT_SUCCESS;
	}
	PrintNg();
	PrintError(result.error_log);
	return EXIT_FAILURE;
}

} // namespace

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

/* namespace内で定義しないと他のコードで使えない */
namespace config {
namespace node {

bool operator==(const node::Node &lhs, const node::Node &rhs) {
	return lhs.token == rhs.token && lhs.token_type == rhs.token_type;
}

bool operator!=(const node::Node &lhs, const node::Node &rhs) {
	return lhs.token != rhs.token || lhs.token_type != rhs.token_type;
}

} // namespace node
} // namespace config

using namespace config;

namespace { /*lexer固有*/

typedef std::list<node::Node> NodeList;

// expected
struct TestCase {
	TestCase(const std::string &input, const NodeList &expected)
		: input(input), expected(expected) {}
	const std::string input;
	const NodeList    expected;
};

std::ostream &operator<<(std::ostream &os, const node::Node node) {
	os << "{token: " << node.token << ", "
	   << "token_type: " << node.token_type << "}";
	return os;
}

std::ostream &operator<<(std::ostream &os, const NodeList &nodes) {
	NodeList::const_iterator it = nodes.begin();
	while (it != nodes.end()) {
		os << *it;
		++it;
		if (it != nodes.end()) {
			os << ", ";
		}
	}
	return os;
}

Result Run(const std::string &src, const NodeList &expected) {
	Result run_result;

	NodeList     nodes;
	lexer::Lexer lex(src, nodes);
	if (nodes != expected) {
		std::ostringstream error_log;
		error_log << "Error: Status Code\n";
		error_log << "- Expected: [ " << expected << " ]\n";
		error_log << "- Result  : [ " << nodes << " ]\n";
		run_result.is_success = false;
		run_result.error_log  = error_log.str();
	}
	return run_result;
}

int HandleResult(const Result &result, const std::string &src) {
	if (result.is_success) {
		PrintOk();
		return EXIT_SUCCESS;
	} else {
		PrintNg();
		std::cerr << utils::color::RED << "ConfigLexer failed:" << utils::color::RESET << std::endl;
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
		ret_code |= HandleResult(Run(test_case.input, test_case.expected), test_case.input);
	}
	return ret_code;
}

} // namespace

int main(int argc, char *argv[]) {
	int ret_code = EXIT_SUCCESS;

	/* Test1 */
	NodeList   expected_result_test_1;
	node::Node expected_test_1_node_1 = {"server", node::CONTEXT};
	node::Node expected_test_1_node_2 = {" ", node::L_BRACKET};
	node::Node expected_test_1_node_3 = {"}", node::R_BRACKET};
	expected_result_test_1.push_back(expected_test_1_node_1);
	expected_result_test_1.push_back(expected_test_1_node_2);
	expected_result_test_1.push_back(expected_test_1_node_3);

	static const TestCase test_cases[] = {
		TestCase("server {\n}\n", expected_result_test_1),
	};

	ret_code |= RunTests(test_cases, ARRAY_SIZE(test_cases));

	return ret_code;
}
