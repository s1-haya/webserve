#include "color.hpp"
#include "custom_const_iterator.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>
#include <list>

namespace { /*テスト汎用*/

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

} // namespace

/*------------------------------------------------------------------*/
/*------------------------------------------------------------------*/

using namespace config;

namespace {

int RunSuccessTest() {
	std::list<int> list;
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.push_back(4);
	list.push_back(5);

	CustomConstIterator<int> it(list.begin(), list.end());

	try {
		++it;
		++it;
		++it;
		++it;
		++it; // これで `end` まで進む
		PrintOk();
		return EXIT_SUCCESS;
	} catch (const std::out_of_range &e) {
		PrintNg();
		utils::Debug("Error message", e.what());
		return EXIT_FAILURE;
	}
}

int RunErrorTest() {
	std::list<int> list;
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.push_back(4);
	list.push_back(5);

	CustomConstIterator<int> it(list.begin(), list.end());

	try {
		++it;
		++it;
		++it;
		++it;
		++it; // これで `end` まで進む
		++it; // ここで例外が発生
		PrintNg();
		return EXIT_FAILURE;
	} catch (const std::out_of_range &e) {
		PrintOk();
		utils::Debug("Error message", e.what());
		return EXIT_SUCCESS;
	}
}

} // namespace

int main() {
	int ret_code = EXIT_SUCCESS;

	ret_code |= RunSuccessTest();
	ret_code |= RunErrorTest();

	return ret_code;
}
