#include "cgi_response_parse.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

using namespace http;

// ==================== Test汎用 ==================== //
namespace {

struct Result {
	Result() : is_success(true) {}
	bool        is_success;
	std::string error_log;
};

int GetTestCaseNum() {
	static int test_case_num = 0;
	++test_case_num;
	return test_case_num;
}

void PrintOk() {
	std::cout << utils::color::GREEN << GetTestCaseNum() << ".[OK]" << utils::color::RESET
			  << std::endl;
}

void PrintNg() {
	std::cerr << utils::color::RED << GetTestCaseNum() << ".[NG] " << utils::color::RESET
			  << std::endl;
}

} // namespace

// ================================================= //

typedef CgiResponseParse::ParsedData   ParsedData;
typedef CgiResponseParse::HeaderFields HeaderFields;

Result CompareHeaderFields(const HeaderFields &expected, const HeaderFields &actual) {
	Result result;
	if (expected.size() != actual.size()) {
		result.is_success = false;
		result.error_log  = "The number of header fields is different.";
		return result;
	}
	for (HeaderFields::const_iterator it = expected.begin(); it != expected.end(); ++it) {
		HeaderFields::const_iterator actual_it = actual.find(it->first);
		if (actual_it == actual.end()) {
			result.is_success = false;
			result.error_log  = "The key does not exist.";
			return result;
		}
		if (it->second != actual_it->second) {
			result.is_success = false;
			result.error_log  = "The value is different.";
			result.error_log += " key: " + it->first + ", expected: " + it->second +
								", actual: " + actual_it->second;
			return result;
		}
	}
	return result;
}

Result CompareParsedData(const ParsedData &expected, const ParsedData &actual) {
	Result result;
	Result header_fields_result = CompareHeaderFields(expected.header_fields, actual.header_fields);
	if (!header_fields_result.is_success) {
		result.is_success = false;
		result.error_log  = header_fields_result.error_log;
		return result;
	}
	if (expected.body != actual.body) {
		result.is_success = false;
		result.error_log  = "The body is different.";
		return result;
	}
	return result;
}

int HandleTestResult(const Result &result) {
	if (result.is_success) {
		PrintOk();
		return EXIT_SUCCESS;
	}
	PrintNg();
	std::cerr << result.error_log << std::endl;
	return EXIT_FAILURE;
}

int TestParseValidResponse() {
	std::string response = "Content-Length: 13\r\nContent-Type: text/plain\r\n\r\nHello, world!";
	CgiResponseParse::ParsedData result = CgiResponseParse::Parse(response);

	CgiResponseParse::ParsedData expected;
	expected.header_fields["Content-Length"] = "13";
	expected.header_fields["Content-Type"]   = "text/plain";
	expected.body                            = "Hello, world!";
	return HandleTestResult(CompareParsedData(expected, result));
}

int TestParseHeaderOnlyResponse() {
	std::string                  response = "Content-Length: 0\r\nContent-Type: text/plain\r\n\r\n";
	CgiResponseParse::ParsedData result   = CgiResponseParse::Parse(response);

	CgiResponseParse::ParsedData expected;
	expected.header_fields["Content-Length"] = "0";
	expected.header_fields["Content-Type"]   = "text/plain";
	expected.body                            = "";
	return HandleTestResult(CompareParsedData(expected, result));
}

int TestParseBodyLongerThanContentLength() {
	std::string response = "Content-Length: 5\r\nContent-Type: text/plain\r\n\r\nHello, world!";
	CgiResponseParse::ParsedData result = CgiResponseParse::Parse(response);

	CgiResponseParse::ParsedData expected;
	expected.header_fields["Content-Length"] = "5";
	expected.header_fields["Content-Type"]   = "text/plain";
	expected.body                            = "Hello";
	return HandleTestResult(CompareParsedData(expected, result));
}

int TestParseNoContentLength() {
	std::string                  response = "Content-Type: text/plain\r\n\r\nHello, world!";
	CgiResponseParse::ParsedData result   = CgiResponseParse::Parse(response);

	CgiResponseParse::ParsedData expected;
	expected.header_fields["Content-Type"] = "text/plain";
	expected.body                          = "Hello, world!";
	return HandleTestResult(CompareParsedData(expected, result));
}

// todo: Error

// void TestParseBodyOnlyResponse() {
// 	CgiResponseParse             parser;
// 	std::string                        response = "\r\n\r\nHello, world!";
// 	CgiResponseParse::ParsedData result   = parser.Parse(response);

// 	if (result.header_fields.size() == 0 && result.body == "Hello, world!") {
// 		std::cout << "testParseBodyOnlyResponse passed\n";
// 	} else {
// 		std::cout << "testParseBodyOnlyResponse failed\n";
// 	}
// }

// void TestParseInvalidResponse() {
// 	http::CgiResponseParse             parser;
// 	std::string                        response = "Invalid Response";
// 	http::CgiResponseParse::ParsedData result   = parser.Parse(response);

// 	if (result.header_fields.size() == 0 && result.body == "") {
// 		std::cout << "testParseInvalidResponse passed\n";
// 	} else {
// 		std::cout << "testParseInvalidResponse failed\n";
// 	}
// }

int main() {
	int ret = EXIT_SUCCESS;

	ret |= TestParseValidResponse();
	ret |= TestParseHeaderOnlyResponse();
	ret |= TestParseBodyLongerThanContentLength();
	ret |= TestParseNoContentLength();
	// testParseBodyOnlyResponse();
	// testParseInvalidResponse();

	return ret;
}
