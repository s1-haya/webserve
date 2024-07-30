#include "tmp_http.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>

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

int main(void) {
	int ret_code = 0;

	// リクエストラインの書式が正しい場合
	http::TmpHttp test1;
	test1.ParseHttpRequestFormat(1, "GET / HTTP/1.1\r\n");
	ret_code |= HandleResult(test1.GetStatusCode(1), http::OK);
	ret_code |= HandleResult(test1.GetIsRequestLineFormat(1), true);

	// リクエストラインの書式が正しいくない場合
	http::TmpHttp test2;
	test2.ParseHttpRequestFormat(1, "GET / \r\n");
	ret_code |= HandleResult(test2.GetStatusCode(1), http::BAD_REQUEST);
	ret_code |= HandleResult(test2.GetIsRequestLineFormat(1), false);

	// CRLNがない場合
	http::TmpHttp test3;
	test3.ParseHttpRequestFormat(1, "GET / ");
	// 本来のステータスコードはRequest Timeout
	ret_code |= HandleResult(test3.GetStatusCode(1), http::OK);
	ret_code |= HandleResult(test3.GetIsRequestLineFormat(1), false);

	// const std::string &expected1 = "OK";
	// ret_code |= HandleResult(test.CreateHttpResponse(1), expected1);
	// ret_code |= HandleResult(test.GetIsHttpRequestFormatComplete(1), false);
	return ret_code;
}
