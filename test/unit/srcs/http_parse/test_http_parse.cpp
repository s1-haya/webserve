#include "color.hpp"
#include "http_parse.hpp"
#include <cstdlib> //EXIT_SUCCESS, EXIT_FAILURE
#include <iostream>

namespace test_http_parse {

namespace {

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

// expected
struct TestCase {
	TestCase(const std::string &tmp_input, const http::HttpRequestResult &tmp_expected)
		: input(tmp_input), expected(tmp_expected) {}
	const std::string             input;
	const http::HttpRequestResult expected;
};

// 　仮 関数名
int assert_request_line(
	std::size_t test_case_num, const http::RequestLine &res, const http::RequestLine &expect
) {
	if (expect.method == res.method && expect.request_target == res.request_target &&
		expect.version == res.version) {
		std::cout << utils::color::GREEN << test_case_num << ".[OK] " << utils::color::RESET
				  << std::endl;
		return EXIT_SUCCESS;
	}
	std::cerr << utils::color::RED << test_case_num << ".[NG] " << utils::color::RESET << std::endl;
	std::cerr << utils::color::RED << "HttpParseClass failed: request_line" << utils::color::RESET
			  << std::endl;
	// 仮: coderabbit
	std::cerr << "Expected method: " << expect.method << ", Actual method: " << res.method << std::endl;
	std::cerr << "Expected request_target: " << expect.request_target << ", Actual request_target: " << res.request_target << std::endl;
	std::cerr << "Expected version: " << expect.version << ", Actual version: " << res.version << std::endl;
	return EXIT_FAILURE;
}

// SplitStr()を実行してexpectedと比較
int Run(
	std::size_t test_case_num, const std::string &src, const http::HttpRequestResult &expected
) {
	const http::HttpRequestResult &result = http::HttpParse::Run(src);
	if (result.status_code == expected.status_code) {
		if (result.status_code == http::OK) {
			// 　仮: OKだった場合はHTTPリクエスト情報をテストしたい
			return (assert_request_line(
				test_case_num, result.request.request_line, expected.request.request_line
			));
		}
		std::cout << utils::color::GREEN << test_case_num << ".[OK] " << utils::color::RESET
				  << std::endl;
		return EXIT_SUCCESS;
	}
	std::cerr << utils::color::RED << test_case_num << ".[NG] " << utils::color::RESET << std::endl;
	std::cerr << utils::color::RED << "HttpParseClass failed: status_code" << utils::color::RESET
			  << std::endl;
	std::cerr << "src     : [" << src << "]" << std::endl;
	std::cerr << "result  : [" << result.status_code << "]" << std::endl;
	std::cerr << "expected: [" << expected.status_code << "]" << std::endl;
	return EXIT_FAILURE;
}

int RunTestCases(const TestCase test_cases[], std::size_t num_test_cases) {
	int ret_code = 0;

	for (std::size_t i = 0; i < num_test_cases; i++) {
		const TestCase test_case = test_cases[i];
		ret_code |= Run(i + 1, test_case.input, test_case.expected);
	}
	return ret_code;
}

} // namespace

int RunTest() {
	int ret_code = 0;

	// HTTPリクエストの書式
	// - 期待したステータスコードかどうかテスト
	// 	- OKの場合はHTTPリクエストの中身もテスト（今回はステータスラインのみ）
	static http::HttpRequestResult expected_1;
	static const http::RequestLine expected_request_1("GET", "/", "HTTP/1.1");
	// 要相談：エラーの場合のクエストラインの出力仕方
	// static const http::RequestLine             expected_request_1("GEl", "/", "HTTP/1.1");
	expected_1.request.request_line = expected_request_1;
	expected_1.status_code          = http::OK;
	// 要相談：失敗した時にenumの数値が出る（のちにステータスコードのenumからステータスコードの文字列を作成するので一旦保留）
	// expected_1.status_code = http::BAD_REQUEST;

	// methodが小文字が含まれてる
	static http::HttpRequestResult expected_2;
	expected_2.status_code = http::BAD_REQUEST;

	// methodがUS-ASCII以外の文字が含まれてる
	static http::HttpRequestResult expected_3;
	expected_3.status_code = http::BAD_REQUEST;

	// 課題要件以外のmethod(大文字のみ)が含まれてる
	static http::HttpRequestResult expected_4;
	expected_4.status_code = http::NOT_IMPLEMENTED;

	static const TestCase test_cases_for_space[] = {
		TestCase("GET / HTTP/1.1", expected_1),
		TestCase("GEt / HTTP/1.1", expected_2),
		TestCase("あGE / HTTP/1.1", expected_3),
		TestCase("HEAD / HTTP/1.1", expected_4),
	};

	ret_code |= RunTestCases(test_cases_for_space, ARRAY_SIZE(test_cases_for_space));
	return ret_code;
}

} // namespace test_http_parse
