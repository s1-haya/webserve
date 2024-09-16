#include "color.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

namespace {

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#define UNUSED            0

// 比較する型
typedef unsigned int Expect;

enum Result {
	SUCCESS,
	FAIL
};

int GetTestCaseNum() {
	static unsigned int test_case_num = 0;
	++test_case_num;
	return test_case_num;
}

struct TestCase {
	TestCase(const std::string &tmp_src, Expect tmp_expected, Result tmp_result)
		: src(tmp_src), expected(tmp_expected), result(tmp_result) {}
	const std::string src;
	const Expect      expected;
	Result            result;
};

// ConvertStrToUint()を実行してexpectedと比較
int Run(const std::string &src, Expect expected, Result result) {
	const utils::Result<Expect> convert_result = utils::ConvertStrToUint(src);
	if ((convert_result.IsOk() && result == SUCCESS && convert_result.GetValue() == expected) ||
		(!convert_result.IsOk() && result == FAIL)) {
		std::cout << utils::color::GREEN << GetTestCaseNum() << ".[OK] " << utils::color::RESET
				  << std::endl;
		return EXIT_SUCCESS;
	}
	std::cerr << utils::color::RED << GetTestCaseNum() << ".[NG] " << utils::color::RESET
			  << std::endl;
	std::cerr << utils::color::RED << "ConvertStrToUint() failed" << utils::color::RESET
			  << std::endl;
	std::cerr << "src     : [" << src << "]" << std::endl;
	std::cerr << "result  : [" << convert_result.GetValue() << "]" << std::endl;
	std::cerr << "expected: [" << expected << "]" << std::endl;
	return EXIT_FAILURE;
}

int RunTestCases(const TestCase test_cases[], std::size_t num_test_cases) {
	int ret_code = 0;

	for (std::size_t i = 0; i < num_test_cases; i++) {
		const TestCase test_case = test_cases[i];
		ret_code |= Run(test_case.src, test_case.expected, test_case.result);
	}
	return ret_code;
}

} // namespace

int main() {
	int ret_code = 0;

	static const TestCase test_cases_for_space[] = {
		// 失敗が返ることを確認するtestcase
		TestCase("", UNUSED, FAIL),
		TestCase("   ", UNUSED, FAIL),
		TestCase("a", UNUSED, FAIL),
		TestCase("8080a", UNUSED, FAIL),
		TestCase("a8080", UNUSED, FAIL),
		TestCase("    8080", UNUSED, FAIL),
		TestCase("8080    ", UNUSED, FAIL),
		TestCase("-8080", UNUSED, FAIL),
		TestCase("4294967296", UNUSED, FAIL), // UINT_MAX + 1

		// 成功を確認するtestcase
		TestCase("0", 0, SUCCESS),
		TestCase("8080", 8080, SUCCESS),
		TestCase("4294967295", 4294967295, SUCCESS), // UINT_MAX
	};

	ret_code |= RunTestCases(test_cases_for_space, ARRAY_SIZE(test_cases_for_space));

	return ret_code;
}
