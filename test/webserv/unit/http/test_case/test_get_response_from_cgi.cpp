#include "client_infos.hpp"
#include "http.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include "test_expected_response.hpp"
#include "test_handler.hpp"
#include "utils.hpp"
#include <cstdlib>

#include "cgi.hpp"

namespace test {

template <typename T>
int HandleResult(const T &result, const T &expected, int number) {
	if (result == expected) {
		std::cout << utils::color::GREEN << number << ".[OK]" << utils::color::RESET << std::endl;
		return EXIT_SUCCESS;
	} else {
		std::cerr << "Error: " << std::endl;
		std::cerr << utils::color::RED << number << ".[NG] " << utils::color::RESET << std::endl;
		std::cerr << "result: [" << result << "]" << std::endl;
		std::cerr << "expected: [" << expected << "]" << std::endl;
		return EXIT_FAILURE;
	}
}

/* これらのテストではHttpStorageは未使用(Parseからの一連の流れは別のテストを作成) */
// 正常なテスト
int TestGetResponseFromCgi1() {
	const std::string &response =
		"Content-Length: 12\r\nContent-Type: text/plain\r\n\r\nHello, world";
	cgi::CgiResponse cgi_response(response, "text/html", true);

	const std::string &expected_body_message = "Hello, world";
	HeaderFields       expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::KEEP_ALIVE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_PLAIN;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        EXPECTED_STATUS_LINE_OK, expected_header_fields, expected_body_message
    );

	http::Http       http;
	http::HttpResult result = http.GetResponseFromCgi(1, cgi_response);
	return HandleResult(result.response, expected_response, 1);
}

// Content-Lengthが無い場合 > ボディの長さをContent-Lengthに設定
int TestGetResponseFromCgi2() {
	const std::string &response =
		"Content-Type: text/html\r\n\r\n<!DOCTYPE "
		"html><html><head><title>Test</title></head><body><h1>Test</h1></body></html>";
	cgi::CgiResponse cgi_response(response, "text/plain", true);

	const std::string &expected_body_message =
		"<!DOCTYPE html><html><head><title>Test</title></head><body><h1>Test</h1></body></html>";
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::KEEP_ALIVE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        EXPECTED_STATUS_LINE_OK, expected_header_fields, expected_body_message
    );

	http::Http       http;
	http::HttpResult result = http.GetResponseFromCgi(2, cgi_response);
	return HandleResult(result.response, expected_response, 2);
}

// Content-Typeが無い場合 > application/octet-streamを設定
int TestGetResponseFromCgi3() {
	const std::string &response = "Content-Length: 12\r\n\r\nHello, world";
	cgi::CgiResponse   cgi_response(response, "text/plain", true);

	const std::string &expected_body_message = "Hello, world";
	HeaderFields       expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::KEEP_ALIVE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::APPLICATION_OCTET_STREAM;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        EXPECTED_STATUS_LINE_OK, expected_header_fields, expected_body_message
    );

	http::Http       http;
	http::HttpResult result = http.GetResponseFromCgi(3, cgi_response);
	return HandleResult(result.response, expected_response, 3);
}

// Content-Lengthが出力より短い場合 > Content-Lengthの長さで切り捨て
int TestGetResponseFromCgi4() {
	const std::string &response =
		"Content-Length: 3\r\nContent-Type: text/plain\r\n\r\nHello, world";
	cgi::CgiResponse cgi_response(response, "text/plain", true);

	const std::string &expected_body_message = "Hel";
	HeaderFields       expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::KEEP_ALIVE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_PLAIN;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        EXPECTED_STATUS_LINE_OK, expected_header_fields, expected_body_message
    );

	http::Http       http;
	http::HttpResult result = http.GetResponseFromCgi(4, cgi_response);
	return HandleResult(result.response, expected_response, 4);
}

} // namespace test
