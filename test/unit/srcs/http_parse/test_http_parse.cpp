#include "color.hpp"
#include "http_parse.hpp"
#include <cstdlib> //EXIT_SUCCESS, EXIT_FAILURE
#include <iostream>
#include <ostream>
#include <sstream> //stringstream

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

bool IsSameRequestLine(
	std::size_t test_case_num, const http::RequestLine &res, const http::RequestLine &expected
) {
	bool              result = true;
	std::stringstream error_log;
	if (expected.method != res.method) {
		error_log << "Expected method: " << expected.method << ", Actual method: " << res.method
				  << "\n";
		result = false;
	}
	if (expected.request_target != res.request_target) {
		error_log << "Expected request_target: " << expected.request_target
				  << ", Actual request_target: " << res.request_target << "\n";
		result = false;
	}
	if (expected.version != res.version) {
		error_log << "Expected version: " << expected.version << ", Actual version: " << res.version
				  << "\n";
		result = false;
	}
	if (result == false) {
		std::cerr << utils::color::RED << test_case_num << ".[NG] " << utils::color::RESET
				  << std::endl;
		std::cerr << utils::color::RED << "HttpParseClass failed: request_line"
				  << utils::color::RESET << std::endl;
		std::cerr << error_log.str();
	}
	return result;
}

bool IsSameHeaderFields(
	std::size_t test_case_num, const http::HeaderFields &res, http::HeaderFields expected
) {
	bool              result = true;
	std::stringstream error_log;
	// 仮
	// expectedがconstじゃない理由は期待値にヘッダフィールドがなければパスして欲しいため
	if (expected["Host"].size() && expected.at("Host") != res.at("Host")) {
		error_log << "Expected Host: " << expected.at("Host") << ", Actual Host: " << res.at("Host")
				  << "\n";
		result = false;
	}
	if (expected["Connection"].size() && expected.at("Connection") != res.at("Connection")) {
		error_log << "Expected Host: " << expected.at("Connection")
				  << ", Actual Connection: " << res.at("Connection") << "\n";
		result = false;
	}
	if (result == false) {
		std::cerr << utils::color::RED << test_case_num << ".[NG] " << utils::color::RESET
				  << std::endl;
		std::cerr << utils::color::RED << "HttpParseClass failed: header fields"
				  << utils::color::RESET << std::endl;
		std::cerr << error_log.str();
	}
	return result;
}

bool IsSameHttpRequest(
	std::size_t test_case_num, const http::HttpRequest &res, const http::HttpRequest &expected
) {
	if (!(IsSameRequestLine(test_case_num, res.request_line, expected.request_line)))
		return false;
	if (!(IsSameHeaderFields(test_case_num, res.header_fields, expected.header_fields)))
		return false;
	return true;
}

// HTTPリクエストの書式
// - 期待したステータスコードかどうかテスト
// 	- ステータスコードがOKの場合はHTTPリクエストの中身もテスト（今回はステータスラインのみ）
int Run(
	std::size_t test_case_num, const std::string &src, const http::HttpRequestResult &expected
) {
	const http::HttpRequestResult result = http::HttpParse::Run(src);
	if (result.status_code == expected.status_code) {
		// 　仮: ステータスコードがOKだった場合はHTTPリクエスト情報をテストしたい
		if (result.status_code == http::OK &&
			!(IsSameHttpRequest(test_case_num, result.request, expected.request)))
			return EXIT_FAILURE;
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

	std::cout << "Request Line Test" << std::endl;
	http::HttpRequestResult        expected_request_line_test_1;
	static const http::RequestLine expected_request_line_1 = {"GET", "/", "HTTP/1.1"};
	// NGの場合
	// static const http::RequestLine             expected_request_line_1("GET", "/d", "HTTP/1.1d");
	expected_request_line_test_1.request.request_line = expected_request_line_1;
	expected_request_line_test_1.status_code          = http::OK;
	// NGの場合
	// expected_request_line_test_1.status_code = http::BAD_REQUEST;

	// methodが小文字が含まれてる
	http::HttpRequestResult expected_request_line_test_2;
	expected_request_line_test_2.status_code = http::BAD_REQUEST;

	// methodがUS-ASCII以外の文字が含まれてる
	http::HttpRequestResult expected_request_line_test_3;
	expected_request_line_test_3.status_code = http::BAD_REQUEST;

	// 課題要件以外のmethod(大文字のみ)が含まれてる
	http::HttpRequestResult expected_request_line_test_4;
	expected_request_line_test_4.status_code = http::NOT_IMPLEMENTED;

	static const TestCase test_cases_for_request_line[] = {
		TestCase("GET / HTTP/1.1", expected_request_line_test_1),
		TestCase("GEt / HTTP/1.1", expected_request_line_test_2),
		TestCase("あGE / HTTP/1.1", expected_request_line_test_3),
		TestCase("HEAD / HTTP/1.1", expected_request_line_test_4),
	};

	ret_code |= RunTestCases(test_cases_for_request_line, ARRAY_SIZE(test_cases_for_request_line));

	std::cout << "Header Fields Test" << std::endl;
	http::HttpRequestResult        expected_header_fields_test_1;
	expected_header_fields_test_1.request.request_line                   = expected_request_line_1;
	expected_header_fields_test_1.status_code                            = http::OK;
	expected_header_fields_test_1.request.header_fields["Host"]          = "www.example.com";
	expected_header_fields_test_1.request.header_fields["Connection"]    = "keep-alive";

	// セミコロン以降に複数OWS(SpaceとHorizontal Tab)が設定の場合
	http::HttpRequestResult        expected_header_fields_test_2;
	expected_header_fields_test_2.request.request_line                   = expected_request_line_1;
	expected_header_fields_test_2.status_code                            = http::OK;
	expected_header_fields_test_2.request.header_fields["Host"]          = "www.example.com";
	expected_header_fields_test_2.request.header_fields["Connection"]    = "keep-alive";

	// セミコロンの後にnameがある設定の場合
	http::HttpRequestResult        expected_header_fields_test_3;
	expected_header_fields_test_3.request.request_line                   = expected_request_line_1;
	expected_header_fields_test_3.status_code                            = http::OK;
	expected_header_fields_test_3.request.header_fields["Host"]          = "www.example.com";
	expected_header_fields_test_3.request.header_fields["Connection"]    = "keep-alive";

	// 存在しないfield_nameの場合
	http::HttpRequestResult expected_header_fileds_test_4;
	expected_header_fileds_test_4.status_code = http::BAD_REQUEST;

	// 複数のヘッダーフィールドの書式が設定の場合
	http::HttpRequestResult expected_header_fileds_test_5;
	expected_header_fileds_test_5.status_code = http::BAD_REQUEST;

	static const TestCase test_cases_for_header_fields[] = {
		TestCase(
			"GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: keep-alive\r\n\r\n", expected_header_fields_test_1
		),
		TestCase(
			"GET / HTTP/1.1\r\nHost:    \twww.example.com\r\nConnection: keep-alive\r\n\r\n",
			expected_header_fields_test_2
		),
		TestCase(
			"GET / HTTP/1.1\r\nHost:www.example.com\r\nConnection: keep-alive\r\n\r\n", expected_header_fields_test_3
		),
		TestCase(
			"GET / HTTP/1.1\r\nGold: www.example.com\r\nConnection: keep-alive\r\n\r\n", expected_header_fileds_test_4
		),
		TestCase(
			"GET / HTTP/1.1\r\nHost: www.example.com\r\nHost: www.example.com\r\nConnection: "
			"keep-alive\r\n\r\n",
			expected_header_fileds_test_5
		)
	};

	ret_code |=
		RunTestCases(test_cases_for_header_fields, ARRAY_SIZE(test_cases_for_header_fields));
	return ret_code;
}

} // namespace test_http_parse
