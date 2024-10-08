#include "http_message.hpp"
#include "http_parse.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {

struct TestCase {
	TestCase(const std::string &read_buf, const http::HttpRequestParsedData &expected)
		: read_buf(read_buf), expected(expected) {}
	const std::string                 read_buf;
	const http::HttpRequestParsedData expected;
};

struct Result {
	Result() : is_success(true) {}
	bool        is_success;
	std::string error_log;
};

http::RequestLine CreateRequestLine(
	const std::string &method, const std::string &request_target, const std::string &version
) {
	http::RequestLine request_line;
	request_line.method         = method;
	request_line.request_target = request_target;
	request_line.version        = version;
	return request_line;
}

int GetTestCaseNum() {
	static unsigned int test_case_num = 0;
	++test_case_num;
	return test_case_num;
}

template <typename T>
bool IsSame(const T &result, const T &expected) {
	return result == expected;
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

Result IsSameHttpRequestFormat(
	const http::IsHttpRequestFormat &result, const http::IsHttpRequestFormat &expected
) {
	Result request_format_result;
	if (!(IsSame(result.is_request_line, expected.is_request_line) &&
		  IsSame(result.is_header_fields, expected.is_header_fields) &&
		  IsSame(result.is_body_message, expected.is_body_message))) {
		std::ostringstream error_log;
		error_log << "Error: Is Http Request Format\n";
		error_log << "Request Line\n";
		error_log << "- Expected: [" << std::boolalpha << expected.is_request_line << "]\n";
		error_log << "- Result  : [" << std::boolalpha << result.is_request_line << "]\n";
		error_log << "Header Fields\n";
		error_log << "- Expected: [" << std::boolalpha << expected.is_header_fields << "]\n";
		error_log << "- Result  : [" << std::boolalpha << result.is_header_fields << "]\n";
		error_log << "Body Message\n";
		error_log << "- Expected: [" << std::boolalpha << expected.is_body_message << "]\n";
		error_log << "- Result  : [" << std::boolalpha << result.is_body_message << "]\n";
		request_format_result.is_success = false;
		request_format_result.error_log  = error_log.str();
	}
	return request_format_result;
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
	if (expected[http::HOST].size() && expected.at(http::HOST) != res.at(http::HOST)) {
		error_log << "Error: Host\n";
		error_log << "- Expected: [" << expected.at(http::HOST) << "]\n";
		error_log << "- Result  : [" << res.at(http::HOST) << "]\n";
		header_fields_result.is_success = false;
	}
	if (expected[http::CONNECTION].size() &&
		expected.at(http::CONNECTION) != res.at(http::CONNECTION)) {
		error_log << "Error: Connection\n";
		error_log << "- Expected: [" << expected.at(http::CONNECTION) << "]\n";
		error_log << "- Result  : [" << res.at(http::CONNECTION) << "]\n";
		header_fields_result.is_success = false;
	}
	if (expected[http::CONTENT_LENGTH].size() &&
		expected.at(http::CONTENT_LENGTH) != res.at(http::CONTENT_LENGTH)) {
		error_log << "Error: Content-Length\n";
		error_log << "- Expected: [" << expected.at(http::CONTENT_LENGTH) << "]\n";
		error_log << "- Result  : [" << res.at(http::CONTENT_LENGTH) << "]\n";
		header_fields_result.is_success = false;
	}
	if (expected[http::TRANSFER_ENCODING].size() &&
		expected.at(http::TRANSFER_ENCODING) != res.at(http::TRANSFER_ENCODING)) {
		error_log << "Error: Transfer-Encoding\n";
		error_log << "- Expected: [" << expected.at(http::TRANSFER_ENCODING) << "]\n";
		error_log << "- Result  : [" << res.at(http::TRANSFER_ENCODING) << "]\n";
		header_fields_result.is_success = false;
	}
	header_fields_result.error_log = error_log.str();
	return header_fields_result;
}

Result IsSameBodyMessage(const std::string &res, const std::string &expected) {
	Result             body_message_result;
	std::ostringstream error_log;
	if (res != expected) {
		error_log << "Error: body_message\n";
		error_log << "- Expected: [" << expected << "]\n";
		error_log << "- Result  : [" << res << "]\n";
		body_message_result.is_success = false;
	}
	body_message_result.error_log = error_log.str();
	return body_message_result;
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
	Result body_message_result = IsSameBodyMessage(res.body_message, expected.body_message);
	if (!(body_message_result.is_success)) {
		return body_message_result;
	}
	Result http_request_result;
	return http_request_result;
}

Result IsSameCurrentBuf(const std::string &current_buf, const std::string &expected_current_buf) {
	Result             current_buf_result;
	std::ostringstream error_log;
	if (current_buf != expected_current_buf) {
		error_log << "Error: current_buf\n";
		error_log << "- Expected: [" << expected_current_buf << "]\n";
		error_log << "- Result  : [" << current_buf << "]\n";
		current_buf_result.is_success = false;
	}
	current_buf_result.error_log = error_log.str();
	return current_buf_result;
}

Result IsSameHttpRequestParsedData(
	const http::HttpRequestParsedData &result, const http::HttpRequestParsedData &expected
) {
	Result status_code_result =
		IsSameStatusCode(result.request_result.status_code, expected.request_result.status_code);
	if (!status_code_result.is_success) {
		return status_code_result;
	}
	Result is_http_request_format_result =
		IsSameHttpRequestFormat(result.is_request_format, expected.is_request_format);
	if (!is_http_request_format_result.is_success) {
		return is_http_request_format_result;
	}
	Result http_request_result;
	bool   is_response_complete = result.is_request_format.is_request_line &&
								result.is_request_format.is_header_fields &&
								result.is_request_format.is_body_message;
	if (is_response_complete) {
		http_request_result =
			IsSameHttpRequest(result.request_result.request, expected.request_result.request);
		if (!http_request_result.is_success) {
			return http_request_result;
		}
	}
	Result current_buf_result = IsSameCurrentBuf(result.current_buf, expected.current_buf);
	return current_buf_result;
}

http::HttpRequestParsedData ParseHttpRequestFormat(const std::string &read_buf) {
	http::HttpRequestParsedData save_data;
	save_data.current_buf += read_buf;
	try {
		http::HttpParse::Run(save_data);
	} catch (const http::HttpException &e) {
		save_data.request_result.status_code = e.GetStatusCode();
	}
	return save_data;
}

int Run(const std::string &read_buf, const http::HttpRequestParsedData &expected) {
	const Result &result = IsSameHttpRequestParsedData(ParseHttpRequestFormat(read_buf), expected);
	return HandleResult(result);
}

int RunTestCases(const TestCase test_cases[], std::size_t num_test_cases) {
	int ret_code = EXIT_SUCCESS;

	for (std::size_t i = 0; i < num_test_cases; i++) {
		const TestCase test_case = test_cases[i];
		ret_code |= Run(test_case.read_buf, test_case.expected);
	}
	return ret_code;
}

http::HttpRequestParsedData ParseHttpRequestFormatForChunked(http::HttpRequestParsedData &save_data
) {
	try {
		http::HttpParse::Run(save_data);
	} catch (const http::HttpException &e) {
		save_data.request_result.status_code = e.GetStatusCode();
	}
	return save_data;
}

Result ParseChunkedMultipleTimes1() {
	http::HttpRequestParsedData save_data;
	// 1回目のread_bufをセット
	save_data.current_buf = "POST / HTTP/1.1\r\nHost: host\r\nTransfer-Encoding: "
							"chunked\r\nContent-Type: text/plain\r\n\r\n4\r\nWi";

	// 1回目のParseのexpected
	http::HttpRequestParsedData expected;
	expected.request_result.status_code          = http::StatusCode(http::OK);
	expected.request_result.request.request_line = CreateRequestLine("POST", "/", "HTTP/1.1");
	expected.is_request_format.is_request_line   = true;
	expected.is_request_format.is_header_fields  = true;
	expected.is_request_format.is_body_message   = false;
	expected.request_result.request.body_message = "";
	expected.current_buf                         = "4\r\nWi";

	// 1回目のParse
	const Result result1 =
		IsSameHttpRequestParsedData(ParseHttpRequestFormatForChunked(save_data), expected);
	if (!result1.is_success) {
		return result1;
	}

	// 次のread_bufが追加される
	save_data.current_buf += "ki\r\n5\r\npedia\r\n0\r\n\r\nGET /";

	// 2回目のParseのexpected
	expected.is_request_format.is_body_message   = true; // OK
	expected.request_result.request.body_message = "Wikipedia";
	expected.current_buf                         = "GET /";

	// 2回目のParse
	const Result result2 =
		IsSameHttpRequestParsedData(ParseHttpRequestFormatForChunked(save_data), expected);
	return result2;
}

Result ParseChunkedMultipleTimes2() {
	http::HttpRequestParsedData save_data;
	// 1回目のread_bufをセット
	save_data.current_buf = "POST / HTTP/1.1\r\nHost: host\r\nTransfer-Encoding: "
							"chunked\r\nContent-Type: text/plain\r\n\r\n4\r\nWiki\r";

	// 1回目のParseのexpected
	http::HttpRequestParsedData expected;
	expected.request_result.status_code          = http::StatusCode(http::OK);
	expected.request_result.request.request_line = CreateRequestLine("POST", "/", "HTTP/1.1");
	expected.is_request_format.is_request_line   = true;
	expected.is_request_format.is_header_fields  = true;
	expected.is_request_format.is_body_message   = false;
	expected.request_result.request.body_message = "";
	expected.current_buf                         = "4\r\nWiki\r";

	// 1回目のParse
	const Result result1 =
		IsSameHttpRequestParsedData(ParseHttpRequestFormatForChunked(save_data), expected);
	if (!result1.is_success) {
		return result1;
	}

	// 次のread_bufが追加される
	save_data.current_buf += "\n5\r\nxx\r\n0\r\n\r\nGET /";

	// 2回目のParseのexpected
	expected.request_result.status_code          = http::StatusCode(http::BAD_REQUEST); // 400
	expected.request_result.request.body_message = "Wiki";
	expected.current_buf                         = "5\r\nxx\r\n0\r\n\r\nGET /";

	// 2回目のParse
	const Result result2 =
		IsSameHttpRequestParsedData(ParseHttpRequestFormatForChunked(save_data), expected);
	return result2;
}

} // namespace

int main(void) {
	int ret_code = EXIT_SUCCESS;

	// todo: http/http_response/test_http_request.cpp HttpRequestParsedData関数のテストケース

	// 1.リクエストラインの書式が正しい場合
	http::HttpRequestParsedData test1_request_line;
	test1_request_line.request_result.status_code        = http::StatusCode(http::OK);
	test1_request_line.is_request_format.is_request_line = true;
	test1_request_line.current_buf                       = "";

	// 2.リクエストラインの書式が正しいくない場合
	http::HttpRequestParsedData test2_request_line;
	test2_request_line.request_result.status_code        = http::StatusCode(http::BAD_REQUEST);
	test2_request_line.is_request_format.is_request_line = false;
	test2_request_line.current_buf                       = "";

	// 3.CRLFがない場合
	http::HttpRequestParsedData test3_request_line;
	// 本来のステータスコードはRequest Timeout
	test3_request_line.request_result.status_code        = http::StatusCode(http::OK);
	test3_request_line.is_request_format.is_request_line = false;
	test3_request_line.current_buf                       = "GET / HTTP/1.1";

	static const TestCase test_case_http_request_line_format[] = {
		TestCase("GET / HTTP/1.1\r\n", test1_request_line),
		TestCase("GET / HTTP/1.\r\n", test2_request_line),
		TestCase("GET / HTTP/1.1", test3_request_line)
	};

	ret_code |= RunTestCases(
		test_case_http_request_line_format,
		sizeof(test_case_http_request_line_format) / sizeof(test_case_http_request_line_format[0])
	);

	// 4.ヘッダフィールドの書式が正しい場合
	http::HttpRequestParsedData test1_header_fields;
	test1_header_fields.request_result.status_code = http::StatusCode(http::OK);
	test1_header_fields.request_result.request.request_line =
		CreateRequestLine("GET", "/", "HTTP/1.1");
	test1_header_fields.is_request_format.is_request_line  = true;
	test1_header_fields.is_request_format.is_header_fields = true;
	test1_header_fields.is_request_format.is_body_message  = true;
	test1_header_fields.current_buf                        = "";

	// 5.ヘッダフィールドの書式が正しくない場合
	http::HttpRequestParsedData test2_header_fields;
	test2_header_fields.request_result.status_code         = http::StatusCode(http::BAD_REQUEST);
	test2_header_fields.is_request_format.is_request_line  = true;
	test2_header_fields.is_request_format.is_header_fields = false;
	test2_header_fields.is_request_format.is_body_message  = false;
	test2_header_fields.current_buf                        = "";

	// 6.ヘッダフィールドにContent-Lengthがあるがボディメッセージがない場合
	http::HttpRequestParsedData test3_header_fields;
	// 本来のステータスコードはRequest Timeout
	test3_header_fields.request_result.status_code = http::StatusCode(http::OK);
	test3_header_fields.request_result.request.request_line =
		CreateRequestLine("GET", "/", "HTTP/1.1");
	test3_header_fields.is_request_format.is_request_line  = true;
	test3_header_fields.is_request_format.is_header_fields = true;
	test3_header_fields.is_request_format.is_body_message  = false;
	test3_header_fields.current_buf                        = "";

	static const TestCase test_case_http_request_header_fields_format[] = {
		TestCase("GET / HTTP/1.1\r\nHost: a\r\n\r\n", test1_header_fields),
		TestCase("GET / HTTP/1.1\r\nHost :\r\n\r\n", test2_header_fields),
		TestCase("GET / HTTP/1.1\r\nHost: test\r\nContent-Length: 2\r\n\r\n", test3_header_fields),
	};

	ret_code |= RunTestCases(
		test_case_http_request_header_fields_format,
		sizeof(test_case_http_request_header_fields_format) /
			sizeof(test_case_http_request_header_fields_format[0])
	);

	// 7.ボディメッセージが正しい場合
	http::HttpRequestParsedData test1_body_message;
	test1_body_message.request_result.status_code = http::StatusCode(http::OK);
	test1_body_message.request_result.request.request_line =
		CreateRequestLine("GET", "/", "HTTP/1.1");
	test1_body_message.is_request_format.is_request_line   = true;
	test1_body_message.is_request_format.is_header_fields  = true;
	test1_body_message.is_request_format.is_body_message   = true;
	test1_body_message.request_result.request.body_message = "abc";
	test1_body_message.current_buf                         = "";

	// 8.Content-Lengthの数値以上にボディメッセージがある場合
	http::HttpRequestParsedData test2_body_message;
	test2_body_message.request_result.status_code = http::StatusCode(http::OK);
	test2_body_message.request_result.request.request_line =
		CreateRequestLine("GET", "/", "HTTP/1.1");
	test2_body_message.is_request_format.is_request_line   = true;
	test2_body_message.is_request_format.is_header_fields  = true;
	test2_body_message.is_request_format.is_body_message   = true;
	test2_body_message.request_result.request.body_message = "ab";
	test2_body_message.current_buf                         = "ccccccccc";

	// 9.Content-Lengthの値の書式が間違ってる場合
	http::HttpRequestParsedData test3_body_message;
	test3_body_message.request_result.status_code          = http::StatusCode(http::BAD_REQUEST);
	test3_body_message.is_request_format.is_request_line   = true;
	test3_body_message.is_request_format.is_header_fields  = false;
	test3_body_message.is_request_format.is_body_message   = false;
	test3_body_message.request_result.request.body_message = "ab";
	test3_body_message.current_buf                         = "abccccccccc";

	// 10.前後にOWSがある場合
	http::HttpRequestParsedData test4_body_message;
	test4_body_message.request_result.status_code = http::StatusCode(http::OK);
	test4_body_message.request_result.request.request_line =
		CreateRequestLine("GET", "/", "HTTP/1.1");
	test4_body_message.is_request_format.is_request_line                = true;
	test4_body_message.is_request_format.is_header_fields               = true;
	test4_body_message.is_request_format.is_body_message                = true;
	test4_body_message.request_result.request.header_fields[http::HOST] = "host";
	test4_body_message.current_buf                                      = "";

	static const TestCase test_case_http_request_body_message_format[] = {
		TestCase("GET / HTTP/1.1\r\nHost: a\r\nContent-Length:  3\r\n\r\nabc", test1_body_message),
		TestCase(
			"GET / HTTP/1.1\r\nHost: test\r\nContent-Length: 2\r\n\r\nabccccccccc",
			test2_body_message
		),
		TestCase(
			"GET / HTTP/1.1\r\nHost: test\r\nContent-Length: dddd\r\n\r\nabccccccccc",
			test3_body_message
		),
		TestCase("GET / HTTP/1.1\r\nHost:    host    \r\n\r\n", test4_body_message),
	};

	ret_code |= RunTestCases(
		test_case_http_request_body_message_format,
		sizeof(test_case_http_request_body_message_format) /
			sizeof(test_case_http_request_body_message_format[0])
	);

	// 11.Chunked Transfer-Encodingの場合
	http::HttpRequestParsedData test1_body_message_chunked;
	test1_body_message_chunked.request_result.status_code = http::StatusCode(http::OK);
	test1_body_message_chunked.request_result.request.request_line =
		CreateRequestLine("POST", "/", "HTTP/1.1");
	test1_body_message_chunked.is_request_format.is_request_line  = true;
	test1_body_message_chunked.is_request_format.is_header_fields = true;
	test1_body_message_chunked.is_request_format.is_body_message  = true;
	test1_body_message_chunked.request_result.request.body_message =
		"Wikipedia is a free online encyclopedia that anyone can edit.";
	test1_body_message_chunked.current_buf = "";

	// 12.Chunked Transfer-Encodingの場合で、chunk-sizeとchunk-dataの大きさが一致していない場合
	http::HttpRequestParsedData test2_body_message_chunked;
	test2_body_message_chunked.request_result.status_code = http::StatusCode(http::BAD_REQUEST);
	test2_body_message_chunked.request_result.request.request_line =
		CreateRequestLine("POST", "/", "HTTP/1.1");
	test2_body_message_chunked.is_request_format.is_request_line   = true;
	test2_body_message_chunked.is_request_format.is_header_fields  = true;
	test2_body_message_chunked.is_request_format.is_body_message   = false;
	test2_body_message_chunked.request_result.request.body_message = "Wikipedia";
	test2_body_message_chunked.current_buf                         = "2\r\npedia\r\n0\r\n\r\n";

	// 13.Chunked Transfer-Encodingの場合で、Content-Lengthがある場合
	http::HttpRequestParsedData test3_body_message_chunked;
	test3_body_message_chunked.request_result.status_code = http::StatusCode(http::BAD_REQUEST);
	test3_body_message_chunked.request_result.request.request_line =
		CreateRequestLine("POST", "/", "HTTP/1.1");
	test3_body_message_chunked.is_request_format.is_request_line   = true;
	test3_body_message_chunked.is_request_format.is_header_fields  = true;
	test3_body_message_chunked.is_request_format.is_body_message   = false;
	test3_body_message_chunked.request_result.request.body_message = "Wikipedia";
	test3_body_message_chunked.current_buf = "4\r\nWiki\r\n5\r\npedia\r\n0\r\n\r\n";

	// 14.Chunked Transfer-Encodingの場合で、終端に0\r\n\r\nがない場合
	http::HttpRequestParsedData test4_body_message_chunked;
	test4_body_message_chunked.request_result.status_code = http::StatusCode(http::OK);
	test4_body_message_chunked.request_result.request.request_line =
		CreateRequestLine("POST", "/", "HTTP/1.1");
	test4_body_message_chunked.is_request_format.is_request_line   = true;
	test4_body_message_chunked.is_request_format.is_header_fields  = true;
	test4_body_message_chunked.is_request_format.is_body_message   = false;
	test4_body_message_chunked.request_result.request.body_message = "Wikipedia";
	test4_body_message_chunked.current_buf                         = "";

	// 15.Chunked Transfer-Encodingの場合で、chunk-sizeが不正な場合(無効な文字)
	http::HttpRequestParsedData test5_body_message_chunked;
	test5_body_message_chunked.request_result.status_code = http::StatusCode(http::BAD_REQUEST);
	test5_body_message_chunked.request_result.request.request_line =
		CreateRequestLine("POST", "/", "HTTP/1.1");
	test5_body_message_chunked.is_request_format.is_request_line   = true;
	test5_body_message_chunked.is_request_format.is_header_fields  = true;
	test5_body_message_chunked.is_request_format.is_body_message   = false;
	test5_body_message_chunked.request_result.request.body_message = "Wikipedia";
	test5_body_message_chunked.current_buf                         = "ss\r\npedia\r\n";

	// 16.Chunked Transfer-Encodingの場合で、chunk-sizeが不正な場合(負の数)
	http::HttpRequestParsedData test6_body_message_chunked;
	test6_body_message_chunked.request_result.status_code = http::StatusCode(http::BAD_REQUEST);
	test6_body_message_chunked.request_result.request.request_line =
		CreateRequestLine("POST", "/", "HTTP/1.1");
	test6_body_message_chunked.is_request_format.is_request_line   = true;
	test6_body_message_chunked.is_request_format.is_header_fields  = true;
	test6_body_message_chunked.is_request_format.is_body_message   = false;
	test6_body_message_chunked.request_result.request.body_message = "Wikipedia";
	test6_body_message_chunked.current_buf                         = "-122\r\npedia\r\n";

	// 17.Chunked Transfer-Encodingの場合で、終端の0\r\n\r\nが中途半端な場合
	http::HttpRequestParsedData test7_body_message_chunked;
	test7_body_message_chunked.request_result.status_code = http::StatusCode(http::OK);
	test7_body_message_chunked.request_result.request.request_line =
		CreateRequestLine("POST", "/", "HTTP/1.1");
	test7_body_message_chunked.is_request_format.is_request_line   = true;
	test7_body_message_chunked.is_request_format.is_header_fields  = true;
	test7_body_message_chunked.is_request_format.is_body_message   = false;
	test7_body_message_chunked.request_result.request.body_message = "Wikipedia";
	test7_body_message_chunked.current_buf                         = "0\r\n";

	// 18.Chunked Transfer-Encodingの場合で
	//    正常にchunk処理が終了した後、current_bufに次のリクエストが入っている場合正しく残るか
	http::HttpRequestParsedData test8_body_message_chunked;
	test8_body_message_chunked.request_result.status_code = http::StatusCode(http::OK);
	test8_body_message_chunked.request_result.request.request_line =
		CreateRequestLine("POST", "/", "HTTP/1.1");
	test8_body_message_chunked.is_request_format.is_request_line   = true;
	test8_body_message_chunked.is_request_format.is_header_fields  = true;
	test8_body_message_chunked.is_request_format.is_body_message   = true;
	test8_body_message_chunked.request_result.request.body_message = "Wikipedia";
	test8_body_message_chunked.current_buf                         = "GET /";

	// 19.Chunked Transfer-Encodingの場合で、0終わりの場合
	http::HttpRequestParsedData test9_body_message_chunked;
	test9_body_message_chunked.request_result.status_code = http::StatusCode(http::OK);
	test9_body_message_chunked.request_result.request.request_line =
		CreateRequestLine("POST", "/", "HTTP/1.1");
	test9_body_message_chunked.is_request_format.is_request_line   = true;
	test9_body_message_chunked.is_request_format.is_header_fields  = true;
	test9_body_message_chunked.is_request_format.is_body_message   = false;
	test9_body_message_chunked.request_result.request.body_message = "Wikipedia";
	test9_body_message_chunked.current_buf                         = "0";

	// 20.Chunked Transfer-Encodingの場合で、0\r終わりの場合
	http::HttpRequestParsedData test10_body_message_chunked;
	test10_body_message_chunked.request_result.status_code = http::StatusCode(http::OK);
	test10_body_message_chunked.request_result.request.request_line =
		CreateRequestLine("POST", "/", "HTTP/1.1");
	test10_body_message_chunked.is_request_format.is_request_line   = true;
	test10_body_message_chunked.is_request_format.is_header_fields  = true;
	test10_body_message_chunked.is_request_format.is_body_message   = false;
	test10_body_message_chunked.request_result.request.body_message = "Wikipedia";
	test10_body_message_chunked.current_buf                         = "0\r";

	// 21.Chunked Transfer-Encodingの場合で、
	//    0\rの後に\nが来てない場合(大きい数字が来てる扱いでOKとした)
	http::HttpRequestParsedData test11_body_message_chunked;
	test11_body_message_chunked.request_result.status_code = http::StatusCode(http::OK);
	test11_body_message_chunked.request_result.request.request_line =
		CreateRequestLine("POST", "/", "HTTP/1.1");
	test11_body_message_chunked.is_request_format.is_request_line   = true;
	test11_body_message_chunked.is_request_format.is_header_fields  = true;
	test11_body_message_chunked.is_request_format.is_body_message   = false;
	test11_body_message_chunked.request_result.request.body_message = "Wikipedia";
	test11_body_message_chunked.current_buf                         = "0\rGET /";

	// 22.Chunked Transfer-Encodingの場合で、0\r\nの後に\r\nが来てない場合
	http::HttpRequestParsedData test12_body_message_chunked;
	test12_body_message_chunked.request_result.status_code = http::StatusCode(http::BAD_REQUEST);
	test12_body_message_chunked.request_result.request.request_line =
		CreateRequestLine("POST", "/", "HTTP/1.1");
	test12_body_message_chunked.is_request_format.is_request_line   = true;
	test12_body_message_chunked.is_request_format.is_header_fields  = true;
	test12_body_message_chunked.is_request_format.is_body_message   = false;
	test12_body_message_chunked.request_result.request.body_message = "Wikipedia";
	test12_body_message_chunked.current_buf                         = "0\r\nGET /";

	// 23.Chunked Transfer-Encodingの場合で、chunk sizeがoverflowしてる場合 / INT_MAX(7fffffff)
	http::HttpRequestParsedData test13_body_message_chunked;
	test13_body_message_chunked.request_result.status_code = http::StatusCode(http::BAD_REQUEST);
	test13_body_message_chunked.request_result.request.request_line =
		CreateRequestLine("POST", "/", "HTTP/1.1");
	test13_body_message_chunked.is_request_format.is_request_line   = true;
	test13_body_message_chunked.is_request_format.is_header_fields  = true;
	test13_body_message_chunked.is_request_format.is_body_message   = false;
	test13_body_message_chunked.request_result.request.body_message = "Wikipedia";
	test13_body_message_chunked.current_buf                         = "80000000\r\naaa";

	// 24.Chunked Transfer-Encodingの場合で、\r\nが含まれずchunk_sizeが8桁より多い場合は早期に400
	http::HttpRequestParsedData test14_body_message_chunked;
	test14_body_message_chunked.request_result.status_code = http::StatusCode(http::BAD_REQUEST);
	test14_body_message_chunked.request_result.request.request_line =
		CreateRequestLine("POST", "/", "HTTP/1.1");
	test14_body_message_chunked.is_request_format.is_request_line   = true;
	test14_body_message_chunked.is_request_format.is_header_fields  = true;
	test14_body_message_chunked.is_request_format.is_body_message   = false;
	test14_body_message_chunked.request_result.request.body_message = "Wikipedia";
	test14_body_message_chunked.current_buf                         = "123456789";

	static const TestCase test_case_http_request_body_message_format_with_chunked[] = {
		TestCase(
			"POST / HTTP/1.1\r\nHost: host\r\nTransfer-Encoding: chunked\r\nContent-Type: "
			"text/plain\r\n\r\n4\r\nWiki\r\n5\r\npedia\r\n0x34\r\n is a free online encyclopedia "
			"that anyone can edit.\r\n0\r\n\r\n",
			test1_body_message_chunked
		),
		TestCase(
			"POST / HTTP/1.1\r\nHost: host\r\nTransfer-Encoding: chunked\r\nContent-Type: "
			"text/plain\r\n\r\n4\r\nWiki\r\n2\r\npedia\r\n0\r\n\r\n",
			test2_body_message_chunked
		),
		TestCase(
			"POST / HTTP/1.1\r\nHost: host\r\nTransfer-Encoding: chunked\r\nContent-Length: "
			"10\r\nContent-Type: text/plain\r\n\r\n4\r\nWiki\r\n5\r\npedia\r\n0\r\n\r\n",
			test3_body_message_chunked
		),
		TestCase(
			"POST / HTTP/1.1\r\nHost: host\r\nTransfer-Encoding: chunked\r\nContent-Type: "
			"text/plain\r\n\r\n4\r\nWiki\r\n5\r\npedia\r\n",
			test4_body_message_chunked
		),
		TestCase(
			"POST / HTTP/1.1\r\nHost: host\r\nTransfer-Encoding: chunked\r\nContent-Type: "
			"text/plain\r\n\r\n4\r\nWiki\r\nss\r\npedia\r\n",
			test5_body_message_chunked
		),
		TestCase(
			"POST / HTTP/1.1\r\nHost: host\r\nTransfer-Encoding: chunked\r\nContent-Type: "
			"text/plain\r\n\r\n4\r\nWiki\r\n-122\r\npedia\r\n",
			test6_body_message_chunked
		),
		TestCase(
			"POST / HTTP/1.1\r\nHost: host\r\nTransfer-Encoding: chunked\r\nContent-Type: "
			"text/plain\r\n\r\n4\r\nWiki\r\n5\r\npedia\r\n0\r\n",
			test7_body_message_chunked
		),
		TestCase(
			"POST / HTTP/1.1\r\nHost: host\r\nTransfer-Encoding: chunked\r\nContent-Type: "
			"text/plain\r\n\r\n4\r\nWiki\r\n5\r\npedia\r\n0\r\n\r\nGET /",
			test8_body_message_chunked
		),
		TestCase(
			"POST / HTTP/1.1\r\nHost: host\r\nTransfer-Encoding: chunked\r\nContent-Type: "
			"text/plain\r\n\r\n4\r\nWiki\r\n5\r\npedia\r\n0",
			test9_body_message_chunked
		),
		TestCase(
			"POST / HTTP/1.1\r\nHost: host\r\nTransfer-Encoding: chunked\r\nContent-Type: "
			"text/plain\r\n\r\n4\r\nWiki\r\n5\r\npedia\r\n0\r",
			test10_body_message_chunked
		),
		TestCase(
			"POST / HTTP/1.1\r\nHost: host\r\nTransfer-Encoding: chunked\r\nContent-Type: "
			"text/plain\r\n\r\n4\r\nWiki\r\n5\r\npedia\r\n0\rGET /",
			test11_body_message_chunked
		),
		TestCase(
			"POST / HTTP/1.1\r\nHost: host\r\nTransfer-Encoding: chunked\r\nContent-Type: "
			"text/plain\r\n\r\n4\r\nWiki\r\n5\r\npedia\r\n0\r\nGET /",
			test12_body_message_chunked
		),
		TestCase(
			"POST / HTTP/1.1\r\nHost: host\r\nTransfer-Encoding: chunked\r\nContent-Type: "
			"text/plain\r\n\r\n4\r\nWiki\r\n5\r\npedia\r\n80000000\r\naaa",
			test13_body_message_chunked
		),
		TestCase(
			"POST / HTTP/1.1\r\nHost: host\r\nTransfer-Encoding: chunked\r\nContent-Type: "
			"text/plain\r\n\r\n4\r\nWiki\r\n5\r\npedia\r\n123456789",
			test14_body_message_chunked
		),
	};

	ret_code |= RunTestCases(
		test_case_http_request_body_message_format_with_chunked,
		sizeof(test_case_http_request_body_message_format_with_chunked) /
			sizeof(test_case_http_request_body_message_format_with_chunked[0])
	);

	// 25. Chunked Transfer-Encodingの場合で、1回目OKで未完成・2回目OK 200で完成
	//     chunk_dataが途中ならchunk_sizeもcurrent_bufに残す
	ret_code |= HandleResult(ParseChunkedMultipleTimes1());

	// 26. Chunked Transfer-Encodingの場合で、1回目OKで未完成・2回目で400
	ret_code |= HandleResult(ParseChunkedMultipleTimes2());

	return ret_code;
}
