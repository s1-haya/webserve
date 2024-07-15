#include "test_convert_str.hpp"
#include "test_split.hpp"
#include <iostream>
#include <string>

namespace {

typedef int (*TestFunction)(void);

int Run(const std::string &title, TestFunction test_func) {
	std::cout << "\n┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
	std::cout << "┃ " << title << std::endl;
	std::cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;

	return test_func();
}

} // namespace

int main() {
	int ret_code = 0;

	// add unit test
	ret_code |= Run("utils/SplitStr()", &test_split::RunTest);
	ret_code |= Run("utils/ConvertStrToUint()", &test_convert_str::RunTest);

	return ret_code;
}
