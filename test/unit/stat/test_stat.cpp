#include "color.hpp"
#include "http.hpp"
#include "stat.hpp"
#include "system_exception.hpp"
#include <exception>
#include <iostream>

namespace {

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

template <typename T>
bool IsSame(const T &result, const T &expected) {
	return result == expected;
}

template <typename T>
int HandleResult(const T &result, const T &expected) {
	if (IsSame(result, expected)) {
		PrintOk();
		return EXIT_SUCCESS;
	} else {
		PrintNg();
		return EXIT_FAILURE;
	}
}
} // namespace

int main(void) {
	int ret_code = EXIT_SUCCESS;
	// const struct stat &makefile_info = test.GetStatBuffer();
	// PrintStatStructMember(makefile_info);

	try {
		http::Stat test("Makefile");
		ret_code |= HandleResult(test.IsRegularFile(), true);
		ret_code |= HandleResult(test.IsReadableFile(), true);
		ret_code |= HandleResult(test.IsWritableFile(), true);
		ret_code |= HandleResult(test.IsDirectory(), false);
		std::cout << "Makefile size: " << test.GetFileSize() << std::endl;
	} catch (const utils::SystemException &e) {
		std::cerr << e.what() << std::endl;
		std::cerr << e.GetErrorNumber() << std::endl;
	}

	try {
		http::Stat test2("no.txt");
	} catch (const utils::SystemException &e) {
		std::cerr << e.what() << std::endl;
		std::cerr << e.GetErrorNumber() << std::endl;
	}
	return ret_code;
}
