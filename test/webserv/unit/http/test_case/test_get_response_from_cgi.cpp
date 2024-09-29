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

int TestGetResponseFromCgi1() {
	const std::string &response = "response";
	cgi::CgiResponse   cgi_response(response, "text/html", true);

	http::HttpRequestParsedData data;
	data.request_result.request.header_fields[http::CONNECTION]     = http::KEEP_ALIVE;
	data.request_result.request.header_fields[http::CONTENT_TYPE]   = "text/html";
	data.request_result.request.header_fields[http::CONTENT_LENGTH] = "8";

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

} // namespace test
