#include "color.hpp"
#include "http_parse.hpp"
#include <cstdlib> //EXIT_SUCCESS, EXIT_FAILURE
#include <iostream>
#include <ostream>
#include <sstream> //stringstream

namespace {

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

// expected
struct TestCase {
	TestCase(const std::string &tmp_input, const http::HttpRequestResult &tmp_expected)
		: input(tmp_input), expected(tmp_expected) {}
	const std::string             input;
	const http::HttpRequestResult expected;
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

Result IsSameRequestLine(const http::RequestLine &res, const http::RequestLine &expected) {
	Result             request_line_result;
	std::ostringstream error_log;
	if (expected.method != res.method) {
		error_log << "Error: Method\n";
		error_log << "- Expected: [" << expected.method << "]\n";
		error_log << "- Result  : [" << res.method << "]\n";
		request_line_result.is_success = false;
	}
	if (expected.request_target != res.request_target) {
		error_log << "Error: Request Target\n";
		error_log << "- Expected: [" << expected.request_target << "]\n";
		error_log << "- Result  : [" << res.request_target << "]\n";
		request_line_result.is_success = false;
	}
	if (expected.version != res.version) {
		error_log << "Error: Version\n";
		error_log << "- Expected: [" << expected.version << "]\n";
		error_log << "- Result  : [" << res.version << "]\n";
		request_line_result.is_success = false;
	}
	request_line_result.error_log = error_log.str();
	return request_line_result;
}

Result IsSameHeaderFields(const http::HeaderFields &res, http::HeaderFields expected) {
	Result             header_fields_result;
	std::ostringstream error_log;
	if (expected["Host"].size() && expected.at("Host") != res.at("Host")) {
		error_log << "Error: Host\n";
		error_log << "- Expected: [" << expected.at("Host") << "]\n";
		error_log << "- Result  : [" << res.at("Host") << "]\n";
		header_fields_result.is_success = false;
	}
	if (expected["Connection"].size() && expected.at("Connection") != res.at("Connection")) {
		error_log << "Error: Connection\n";
		error_log << "- Expected: [" << expected.at("Connection") << "]\n";
		error_log << "- Result  : [" << res.at("Connection") << "]\n";
		header_fields_result.is_success = false;
	}
	header_fields_result.error_log = error_log.str();
	return header_fields_result;
}

Result
IsSameHttpRequest(const http::HttpRequestFormat &res, const http::HttpRequestFormat &expected) {
	Result request_line_result = IsSameRequestLine(res.request_line, expected.request_line);
	if (!(request_line_result.is_success)) {
		return request_line_result;
	}
	Result header_fields_result = IsSameHeaderFields(res.header_fields, expected.header_fields);
	if (!(header_fields_result.is_success)) {
		return header_fields_result;
	}
	Result http_request_result;
	return http_request_result;
}

Result IsSameStatusCode(const http::StatusCode &status_code, const http::StatusCode &expected) {
	Result status_code_result;
	if (status_code.GetEStatusCode() != expected.GetEStatusCode()) {
		std::ostringstream error_log;
		error_log << "Error: Status Code\n";
		error_log << "- Expected: [" << expected.GetStatusCode() << "]\n";
		error_log << "- Result  : [" << status_code.GetStatusCode() << "]\n";
		status_code_result.is_success = false;
		status_code_result.error_log  = error_log.str();
	}
	return status_code_result;
}

int HandleResult(const Result &result, const std::string &src) {
	if (result.is_success) {
		std::cout << utils::color::GREEN << GetTestCaseNum() << ".[OK] " << utils::color::RESET
				  << std::endl;
		return EXIT_SUCCESS;
	} else {
		std::cerr << utils::color::RED << GetTestCaseNum() << ".[NG] " << utils::color::RESET
				  << std::endl;
		std::cerr << utils::color::RED << "HttpParseClass failed:" << utils::color::RESET
				  << std::endl;
		std::cerr << "src:\n[" << src << "]" << std::endl;
		std::cerr << "- - - - - - - - " << std::endl;
		std::cerr << result.error_log;
		return EXIT_FAILURE;
	}
}

// HTTPリクエストの書式
// - 期待したステータスコードかどうかテスト
// 	- ステータスコードがOKの場合はHTTPリクエストの中身もテスト（今回はステータスラインのみ）
int Run(const std::string &src, const http::HttpRequestResult &expected) {
	const http::HttpRequestResult result = http::HttpParse::Run(src);
	Result status_code_result = IsSameStatusCode(result.status_code, expected.status_code);
	if (status_code_result.is_success && result.status_code.GetEStatusCode() == http::OK) {
		Result http_request_result = IsSameHttpRequest(result.request, expected.request);
		return HandleResult(http_request_result, src);
	} else {
		return HandleResult(status_code_result, src);
	}
}

int RunTestCases(const TestCase test_cases[], std::size_t num_test_cases) {
	int ret_code = 0;

	for (std::size_t i = 0; i < num_test_cases; i++) {
		const TestCase test_case = test_cases[i];
		ret_code |= Run(test_case.input, test_case.expected);
	}
	return ret_code;
}

} // namespace

int main(void) {
	int ret_code = 0;

	std::cout << "Request Line Test" << std::endl;
	http::HttpRequestResult        expected_request_line_test_1;
	static const http::RequestLine expected_request_line_1 = {"GET", "/", "HTTP/1.1"};
	// NGの場合
	// static const http::RequestLine             expected_request_line_1("GET", "/d", "HTTP/1.1d");
	expected_request_line_test_1.request.request_line = expected_request_line_1;
	expected_request_line_test_1.status_code          = http::StatusCode(http::OK);
	// NGの場合
	// expected_request_line_test_1.status_code = http::StatusCode(http::BAD_REQUEST);

	// methodが小文字が含まれてる
	http::HttpRequestResult expected_request_line_test_2;
	expected_request_line_test_2.status_code = http::StatusCode(http::BAD_REQUEST);

	// methodがUS-ASCII以外の文字が含まれてる
	http::HttpRequestResult expected_request_line_test_3;
	expected_request_line_test_3.status_code = http::StatusCode(http::BAD_REQUEST);

	static const TestCase test_cases_for_request_line[] = {
		TestCase("GET / HTTP/1.1", expected_request_line_test_1),
		TestCase("GEt / HTTP/1.1", expected_request_line_test_2),
		TestCase("あGE / HTTP/1.1", expected_request_line_test_3)
	};

	ret_code |= RunTestCases(test_cases_for_request_line, ARRAY_SIZE(test_cases_for_request_line));

	std::cout << "Header Fields Test" << std::endl;
	http::HttpRequestResult expected_header_fields_test_1;
	expected_header_fields_test_1.request.request_line                = expected_request_line_1;
	expected_header_fields_test_1.status_code                         = http::StatusCode(http::OK);
	expected_header_fields_test_1.request.header_fields["Host"]       = "www.example.com";
	expected_header_fields_test_1.request.header_fields["Connection"] = "keep-alive";

	// セミコロン以降に複数OWS(SpaceとHorizontal Tab)が設定の場合
	http::HttpRequestResult expected_header_fields_test_2;
	expected_header_fields_test_2.request.request_line                = expected_request_line_1;
	expected_header_fields_test_2.status_code                         = http::StatusCode(http::OK);
	expected_header_fields_test_2.request.header_fields["Host"]       = "www.example.com";
	expected_header_fields_test_2.request.header_fields["Connection"] = "keep-alive";

	// セミコロンの後にnameがある設定の場合
	http::HttpRequestResult expected_header_fields_test_3;
	expected_header_fields_test_3.request.request_line                = expected_request_line_1;
	expected_header_fields_test_3.status_code                         = http::StatusCode(http::OK);
	expected_header_fields_test_3.request.header_fields["Host"]       = "www.example.com";
	expected_header_fields_test_3.request.header_fields["Connection"] = "keep-alive";

	// 存在しないfield_nameの場合
	http::HttpRequestResult expected_header_fields_test_4;
	expected_header_fields_test_4.status_code = http::StatusCode(http::BAD_REQUEST);

	// 複数のヘッダーフィールドの書式が設定の場合
	http::HttpRequestResult expected_header_fields_test_5;
	expected_header_fields_test_5.status_code = http::StatusCode(http::BAD_REQUEST);

	// セミコロンがない場合
	http::HttpRequestResult expected_header_fileds_test_6;
	expected_header_fileds_test_6.status_code = http::StatusCode(http::BAD_REQUEST);

	// セミコロンが複数ある場合
	http::HttpRequestResult expected_header_fields_test_7;
	expected_header_fields_test_7.status_code = http::StatusCode(http::BAD_REQUEST);

	static const TestCase test_cases_for_header_fields[] = {
		TestCase(
			"GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: keep-alive\r\n\r\n",
			expected_header_fields_test_1
		),
		TestCase(
			"GET / HTTP/1.1\r\nHost:    \twww.example.com\r\nConnection: keep-alive\r\n\r\n",
			expected_header_fields_test_2
		),
		TestCase(
			"GET / HTTP/1.1\r\nHost:www.example.com\r\nConnection: keep-alive\r\n\r\n",
			expected_header_fields_test_3
		),
		TestCase(
			"GET / HTTP/1.1\r\nGold: www.example.com\r\nConnection: keep-alive\r\n\r\n",
			expected_header_fields_test_4
		),
		TestCase(
			"GET / HTTP/1.1\r\nHost: www.example.com\r\nHost: www.example.com\r\nConnection: "
			"keep-alive\r\n\r\n",
			expected_header_fields_test_5
		),
		TestCase(
			"GET / HTTP/1.1\r\nHost\r\nConnection: "
			"keep-alive\r\n\r\n",
			expected_header_fileds_test_6
		),
		TestCase(
			"GET / HTTP/1.1\r\nHost:kkk:kk\r\nConnection: "
			"keep-alive\r\n\r\n",
			expected_header_fields_test_7
		)
	};

	ret_code |=
		RunTestCases(test_cases_for_header_fields, ARRAY_SIZE(test_cases_for_header_fields));
	return ret_code;
}
