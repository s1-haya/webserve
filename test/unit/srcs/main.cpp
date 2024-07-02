#include "test_split.hpp"
#include <iostream>
#include <string>

namespace {

typedef int (*TestFunction)(void);

int Run(const std::string &title, TestFunction test_func) {
	std::cout << "\n\n┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
	std::cout << "┃ " << title << std::endl;
	std::cout << "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;

	return test_func();
}

} // namespace

int main() {
	int ret_code = 0;

	ret_code |= Run("utils/SplitStr()", &test_split::RunTest);

	return ret_code;
}
