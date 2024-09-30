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
int TestGetResponseFromCgi1() {
	const std::string &response = "response";
	cgi::CgiResponse   cgi_response(response, "text/html", true);

	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::KEEP_ALIVE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(response.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response =
		CreateHttpResponseFormat(EXPECTED_STATUS_LINE_OK, expected_header_fields, response);

	http::Http       http;
	http::HttpResult result = http.GetResponseFromCgi(1, cgi_response);
	return HandleResult(result.response, expected_response, 1);
}

int TestGetResponseFromCgi2() {
	const std::string &response = "Hello World";
	cgi::CgiResponse   cgi_response(response, "text/plain", true);

	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::KEEP_ALIVE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(response.length());
	expected_header_fields[http::CONTENT_TYPE]   = "text/plain"; // todo: http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response =
		CreateHttpResponseFormat(EXPECTED_STATUS_LINE_OK, expected_header_fields, response);

	http::Http       http;
	http::HttpResult result = http.GetResponseFromCgi(2, cgi_response);
	return HandleResult(result.response, expected_response, 2);
}

} // namespace test
