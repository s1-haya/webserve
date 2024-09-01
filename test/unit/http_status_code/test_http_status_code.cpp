#include "status_code.hpp"
#include "utils.hpp"
#include <cstdlib>

namespace {

int GetTestCaseNum() {
	static unsigned int test_case_num = 0;
	++test_case_num;
	return test_case_num;
}

template <typename T>
int HandleResult(const T &result, const T &expected) {
	if (result == expected) {
		std::cout << utils::color::GREEN << GetTestCaseNum() << ".[OK]" << utils::color::RESET
				  << std::endl;
		return EXIT_SUCCESS;
	} else {
		std::cerr << utils::color::RED << GetTestCaseNum() << ".[NG] " << utils::color::RESET
				  << std::endl;
		return EXIT_FAILURE;
	}
}

} // namespace

int main(void) {
	int ret_code = EXIT_SUCCESS;

	http::StatusCode   ok(http::OK);
	const std::string &expected_200 = "200";
	const std::string &expected_ok  = "OK";
	ret_code |= HandleResult(ok.GetEStatusCode(), http::OK);
	ret_code |= HandleResult(ok.GetStatusCode(), expected_200);
	ret_code |= HandleResult(ok.GetReasonPhrase(), expected_ok);

	return ret_code;
}
