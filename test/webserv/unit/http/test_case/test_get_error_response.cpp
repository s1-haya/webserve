#include "client_infos.hpp"
#include "http.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include "test_expected_response.hpp"
#include "test_handler.hpp"
#include "utils.hpp"
#include <cstdlib>

namespace test {

template <typename T>
int HandleGetErrorResponseError(const T &result, const T &expected, int number) {
	if (result == expected) {
		std::cout << utils::color::GREEN << number << ".[OK]" << utils::color::RESET << std::endl;
		return EXIT_SUCCESS;
	} else {
		std::cerr << "Error: GetErrorResponse" << std::endl;
		std::cerr << utils::color::RED << number << ".[NG] " << utils::color::RESET << std::endl;
		std::cerr << "result: [" << result << "]" << std::endl;
		std::cerr << "expected: [" << expected << "]" << std::endl;
		return EXIT_FAILURE;
	}
}

int TestRequestTimeoutResponse() {
	http::ClientInfos client_infos          = CreateClientInfos("");
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_TIMEOUT;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_TIMEOUT;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );

	http::Http       http;
	http::HttpResult result = http.GetErrorResponse(client_infos.fd, http::TIMEOUT);
	return HandleGetErrorResponseError(result.response, expected_response, 1);
}

int TestInternalServerErrorResponse() {
	http::ClientInfos client_infos          = CreateClientInfos("");
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_INTERNAL_SERVER_ERROR;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_INTERNAL_SERVER_ERROR;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );

	http::Http       http;
	http::HttpResult result = http.GetErrorResponse(client_infos.fd, http::INTERNAL_ERROR);
	return HandleGetErrorResponseError(result.response, expected_response, 2);
}

} // namespace test
