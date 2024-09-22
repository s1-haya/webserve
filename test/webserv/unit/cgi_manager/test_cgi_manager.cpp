#include "cgi_manager.hpp"
#include "http_exception.hpp" // todo: remove
#include "http_message.hpp"
#include "status_code.hpp" // todo: remove
#include "utils.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>

namespace {

struct Result {
	Result() : is_success(true) {}
	bool        is_success;
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
	if (result.is_success) {
		PrintOk();
		return EXIT_SUCCESS;
	}
	PrintNg();
	PrintError(result.error_log);
	return EXIT_FAILURE;
}

} // namespace

int main() {
	int ret_code = EXIT_SUCCESS;

	Result result;
	if (!IsSame("a", "a")) {
		result.is_success = false;
	}
	ret_code |= Test(result);

	return ret_code;
}
