#ifndef TEST_EXPECTED_RESPONSE_HPP_
#define TEST_EXPECTED_RESPONSE_HPP_

#include <string>

namespace test {

std::string LoadFileContent(const std::string &file_path);

static const std::string &EXPECTED_RESPONSE = "../../expected_response";
// status_line
static const std::string &EXPECTED_STATUS_LINE = EXPECTED_RESPONSE + "/default_status_line";
static const std::string &EXPECTED_STATUS_LINE_OK =
	LoadFileContent(EXPECTED_STATUS_LINE + "/200_ok.txt");
static const std::string &EXPECTED_STATUS_LINE_BAD_REQUEST =
	LoadFileContent(EXPECTED_STATUS_LINE + "/400_bad_request.txt");
static const std::string &EXPECTED_STATUS_LINE_NOT_FOUND =
	LoadFileContent(EXPECTED_STATUS_LINE + "/404_not_found.txt");
static const std::string &EXPECTED_STATUS_LINE_METHOD_NOT_ALLOWED =
	LoadFileContent(EXPECTED_STATUS_LINE + "/405_method_not_allowed.txt");
static const std::string &EXPECTED_STATUS_LINE_TIMEOUT =
	LoadFileContent(EXPECTED_STATUS_LINE + "/408_timeout.txt");
static const std::string &EXPECTED_STATUS_LINE_INTERNAL_SERVER_ERROR =
	LoadFileContent(EXPECTED_STATUS_LINE + "/500_internal_server_error.txt");
static const std::string &EXPECTED_STATUS_LINE_NOT_IMPLEMENTED =
	LoadFileContent(EXPECTED_STATUS_LINE + "/501_not_implemented.txt");

// body_message
static const std::string &EXPECTED_BODY_MESSAGE = EXPECTED_RESPONSE + "/default_body_message";
static const std::string &EXPECTED_BODY_MESSAGE_BAD_REQUEST =
	LoadFileContent(EXPECTED_BODY_MESSAGE + "/400_bad_request.txt");
static const std::string &EXPECTED_BODY_MESSAGE_NOT_FOUND =
	LoadFileContent(EXPECTED_BODY_MESSAGE + "/404_not_found.txt");
static const std::string &EXPECTED_BODY_MESSAGE_METHOD_NOT_ALLOWED =
	LoadFileContent(EXPECTED_BODY_MESSAGE + "/405_method_not_allowed.txt");
static const std::string &EXPECTED_BODY_MESSAGE_TIMEOUT =
	LoadFileContent(EXPECTED_BODY_MESSAGE + "/408_timeout.txt");
static const std::string &EXPECTED_BODY_MESSAGE_INTERNAL_SERVER_ERROR =
	LoadFileContent(EXPECTED_BODY_MESSAGE + "/500_internal_server_error.txt");
static const std::string &EXPECTED_BODY_MESSAGE_NOT_IMPLEMENTED =
	LoadFileContent(EXPECTED_BODY_MESSAGE + "/501_not_implemented.txt");

} // namespace test

#endif
