#include "tmp_http.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>

namespace {

struct TestCase {
	TestCase(
		int client_fd, const std::string &read_buf, const http::HttpRequestParsedData &expected
	)
		: client_fd(client_fd), read_buf(read_buf), expected(expected) {}
	int                               client_fd;
	const std::string                 read_buf;
	const http::HttpRequestParsedData expected;
};

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

int HandleResult(const Result &result) {
	if (result.is_success) {
		std::cout << utils::color::GREEN << GetTestCaseNum() << ".[OK]" << utils::color::RESET
				  << std::endl;
		return EXIT_SUCCESS;
	} else {
		std::cerr << utils::color::RED << GetTestCaseNum() << ".[NG] " << utils::color::RESET
				  << std::endl;
		std::cerr << result.error_log;
		return EXIT_FAILURE;
	}
}

Result IsHttpRequestParsedData(
	const http::HttpRequestParsedData &result, const http::HttpRequestParsedData &expected
) {
	Result             request_parsed_result;
	std::ostringstream error_log;
	if (result.request_result.status_code != expected.request_result.status_code) {
		error_log << "Error: StatusCode\n";
		error_log << "- Expected: [" << expected.request_result.status_code << "]\n";
		error_log << "- Result  : [" << result.request_result.status_code << "]\n";
		request_parsed_result.is_success = false;
	}
	request_parsed_result.error_log = error_log.str();
	return request_parsed_result;
}

int Run(int client_fd, const std::string &read_buf, const http::HttpRequestParsedData &expected) {
	http::TmpHttp http;
	http.ParseHttpRequestFormat(client_fd, read_buf);
	const Result &result = IsHttpRequestParsedData(http.GetClientData(client_fd), expected);
	return HandleResult(result);
}

int RunTestCases(const TestCase test_cases[], std::size_t num_test_cases) {
	int ret_code = 0;

	for (std::size_t i = 0; i < num_test_cases; i++) {
		const TestCase test_case = test_cases[i];
		ret_code |= Run(test_case.client_fd, test_case.read_buf, test_case.expected);
	}
	return ret_code;
}

} // namespace

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

	// ヘッダフィールドの書式が正しい場合
	http::TmpHttp test1_header_fileds;
	test1_header_fileds.ParseHttpRequestFormat(1, "GET / HTTP/1.1\r\nHost: a\r\n\r\n");
	ret_code |= HandleResult(test1_header_fileds.GetStatusCode(1), http::OK);
	ret_code |= HandleResult(test1_header_fileds.GetIsRequestLineFormat(1), true);
	ret_code |= HandleResult(test1_header_fileds.GetIsHeaderFieldsFormat(1), true);
	ret_code |= HandleResult(test1_header_fileds.GetIsBodyMessageFormat(1), true);

	// ヘッダフィールドの書式が正しくない場合
	http::TmpHttp test2_header_fileds;
	test2_header_fileds.ParseHttpRequestFormat(1, "GET / HTTP/1.1\r\nHost :\r\n\r\n");
	ret_code |= HandleResult(test2_header_fileds.GetStatusCode(1), http::BAD_REQUEST);
	ret_code |= HandleResult(test2_header_fileds.GetIsRequestLineFormat(1), true);
	ret_code |= HandleResult(test2_header_fileds.GetIsHeaderFieldsFormat(1), false);
	ret_code |= HandleResult(test2_header_fileds.GetIsBodyMessageFormat(1), false);

	// ヘッダフィールドにContent-Lengthがある場合
	http::TmpHttp test3_header_fileds;
	test3_header_fileds.ParseHttpRequestFormat(
		1, "GET / HTTP/1.1\r\nHost: test\r\nContent-Length: 2\r\n\r\nab"
	);
	ret_code |= HandleResult(test3_header_fileds.GetStatusCode(1), http::OK);
	ret_code |= HandleResult(test3_header_fileds.GetIsRequestLineFormat(1), true);
	ret_code |= HandleResult(test3_header_fileds.GetIsHeaderFieldsFormat(1), true);
	ret_code |= HandleResult(test3_header_fileds.GetIsBodyMessageFormat(1), true);

	// Content-Lengthの数値以上にボディメッセージがある場合
	http::TmpHttp test4_header_fileds;
	test4_header_fileds.ParseHttpRequestFormat(
		1, "GET / HTTP/1.1\r\nHost: test\r\nContent-Length: 2\r\n\r\nabccccccccc"
	);
	const std::string &test4_body_message = "ab";
	ret_code |= HandleResult(test4_header_fileds.GetStatusCode(1), http::OK);
	ret_code |= HandleResult(test4_header_fileds.GetIsRequestLineFormat(1), true);
	ret_code |= HandleResult(test4_header_fileds.GetIsHeaderFieldsFormat(1), true);
	ret_code |= HandleResult(test4_header_fileds.GetIsBodyMessageFormat(1), true);
	ret_code |= HandleResult(test4_header_fileds.GetBodyMessage(1), test4_body_message);

	http::TmpHttp test5_header_fileds;
	test5_header_fileds.ParseHttpRequestFormat(
		1, "GET / HTTP/1.1\r\nHost: test\r\nContent-Length: dddd\r\n\r\nabccccccccc"
	);
	ret_code |= HandleResult(test5_header_fileds.GetStatusCode(1), http::BAD_REQUEST);
	ret_code |= HandleResult(test5_header_fileds.GetIsRequestLineFormat(1), true);
	ret_code |= HandleResult(test5_header_fileds.GetIsHeaderFieldsFormat(1), true);
	ret_code |= HandleResult(test5_header_fileds.GetIsBodyMessageFormat(1), false);

	// ボディメッセージを追加で送信した場合
	http::TmpHttp test6_header_fileds;
	test6_header_fileds.ParseHttpRequestFormat(
		1, "GET / HTTP/1.1\r\nHost: test\r\nContent-Length:  3\r\n\r\na"
	);
	ret_code |= HandleResult(test6_header_fileds.GetStatusCode(1), http::OK);
	ret_code |= HandleResult(test6_header_fileds.GetIsRequestLineFormat(1), true);
	ret_code |= HandleResult(test6_header_fileds.GetIsHeaderFieldsFormat(1), true);
	ret_code |= HandleResult(test6_header_fileds.GetIsBodyMessageFormat(1), false);
	test6_header_fileds.ParseHttpRequestFormat(1, "bc");
	const std::string &test6_body_message = "abc";
	ret_code |= HandleResult(test6_header_fileds.GetIsBodyMessageFormat(1), true);
	ret_code |= HandleResult(test6_header_fileds.GetBodyMessage(1), test6_body_message);

	http::HttpRequestParsedData test1_request_line;
	test1_request_line.request_result.status_code = http::OK;

	http::HttpRequestParsedData test2_request_line;
	test2_request_line.request_result.status_code = http::BAD_REQUEST;

	static const TestCase test_case_http_request_format[] = {
		TestCase(1, "GET / HTTP/1.1\r\n", test1_request_line),
		TestCase(1, "GET / HTTP/1.\r\n", test2_request_line)
	};
	ret_code |= RunTestCases(
		test_case_http_request_format,
		sizeof(test_case_http_request_format) / sizeof(test_case_http_request_format[0])
	);
	return ret_code;
}
