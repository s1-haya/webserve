#include "cgi_response_parse.hpp"
#include "result.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

using namespace cgi;

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
		result.error_log  = "The number of header fields is different, expected: " +
						   utils::ToString(expected.size()) +
						   ", actual: " + utils::ToString(actual.size());
		return result;
	}
	for (HeaderFields::const_iterator it = expected.begin(); it != expected.end(); ++it) {
		HeaderFields::const_iterator actual_it = actual.find(it->first);
		if (actual_it == actual.end()) {
			result.is_success = false;
			result.error_log  = "The key does not exist: " + it->first;
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
	utils::Result<CgiResponseParse::ParsedData> result = CgiResponseParse::Parse(response);
	if (!result.IsOk()) {
		PrintNg();
		return EXIT_FAILURE;
	}

	CgiResponseParse::ParsedData expected;
	expected.header_fields["Content-Length"] = "13";
	expected.header_fields["Content-Type"]   = "text/plain";
	expected.body                            = "Hello, world!";
	return HandleTestResult(CompareParsedData(expected, result.GetValue()));
}

int TestParseHeaderOnlyResponse() {
	std::string response = "Content-Length: 0\r\nContent-Type: text/plain\r\n\r\n";
	utils::Result<CgiResponseParse::ParsedData> result = CgiResponseParse::Parse(response);
	if (!result.IsOk()) {
		PrintNg();
		return EXIT_FAILURE;
	}

	CgiResponseParse::ParsedData expected;
	expected.header_fields["Content-Length"] = "0";
	expected.header_fields["Content-Type"]   = "text/plain";
	expected.body                            = "";
	return HandleTestResult(CompareParsedData(expected, result.GetValue()));
}

int TestParseBodyLongerThanContentLength() {
	std::string response = "Content-Length: 5\r\nContent-Type: text/plain\r\n\r\nHello, world!";
	utils::Result<CgiResponseParse::ParsedData> result = CgiResponseParse::Parse(response);
	if (!result.IsOk()) {
		PrintNg();
		return EXIT_FAILURE;
	}

	CgiResponseParse::ParsedData expected;
	expected.header_fields["Content-Length"] = "5";
	expected.header_fields["Content-Type"]   = "text/plain";
	expected.body                            = "Hello";
	return HandleTestResult(CompareParsedData(expected, result.GetValue()));
}

int TestParseNoContentLength() {
	std::string response = "Content-Type: text/plain\r\n\r\nHello, world!";
	utils::Result<CgiResponseParse::ParsedData> result = CgiResponseParse::Parse(response);
	if (!result.IsOk()) {
		PrintNg();
		return EXIT_FAILURE;
	}

	CgiResponseParse::ParsedData expected;
	expected.header_fields["Content-Type"] = "text/plain";
	expected.body                          = "Hello, world!";
	return HandleTestResult(CompareParsedData(expected, result.GetValue()));
}

int TestHeaderFieldWithOws() {
	std::string response =
		"Content-Length:    5   \r\nContent-Type: text/plain\r\n\r\nHello, world!";
	utils::Result<CgiResponseParse::ParsedData> result = CgiResponseParse::Parse(response);
	if (!result.IsOk()) {
		PrintNg();
		return EXIT_FAILURE;
	}

	CgiResponseParse::ParsedData expected;
	expected.header_fields["Content-Length"] = "5";
	expected.header_fields["Content-Type"]   = "text/plain";
	expected.body                            = "Hello";
	return HandleTestResult(CompareParsedData(expected, result.GetValue()));
}

int TestErrorResponse(const std::string &response) {
	utils::Result<CgiResponseParse::ParsedData> result = CgiResponseParse::Parse(response);
	if (!result.IsOk()) {
		PrintOk();
		return EXIT_SUCCESS;
	}
	PrintNg();
	return EXIT_FAILURE;
}

int main() {
	int ret = EXIT_SUCCESS;

	ret |= TestParseValidResponse();
	ret |= TestParseHeaderOnlyResponse();
	ret |= TestParseBodyLongerThanContentLength();
	ret |= TestParseNoContentLength();
	ret |= TestHeaderFieldWithOws();
	// ヘッダーフィールドが存在しない
	ret |= TestErrorResponse("Hello, world!");
	ret |= TestErrorResponse("Content-Type: text/plain\r\nHello, world!");
	ret |= TestErrorResponse("\r\n\r\nHello, world!");
	// Content-Lengthが無効
	ret |= TestErrorResponse("Content-Length: -1\r\n\r\nHello, world!");
	ret |= TestErrorResponse("Content-Length: aa\r\n\r\nHello, world!");
	// ヘッダーフィールドのヘッダー名にスペースが含まれている
	ret |= TestErrorResponse("Content-Type   : text/plain\r\n\r\nHello, world!");
	ret |= TestErrorResponse("  Content-Type: text/plain\r\n\r\nHello, world!");
	// ヘッダーフィールドに可視文字でないものが含まれている
	ret |= TestErrorResponse("Content-T\bype: text/plain\r\n\r\nHello, world!");
	// ヘッダーフィールドのヘッダー名が空
	ret |= TestErrorResponse(": 5\r\n\r\nHello, world!");
	// ヘッダーフィールドのヘッダー値が空
	ret |= TestErrorResponse("Content-Length: \r\n\r\nHello, world!");

	return ret;
}
