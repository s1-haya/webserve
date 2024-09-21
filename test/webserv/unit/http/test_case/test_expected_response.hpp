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
static const std::string &EXPECTED_STATUS_LINE_NOT_FOUND =
	LoadFileContent(EXPECTED_STATUS_LINE + "/404_not_found.txt");
// body_message
static const std::string &EXPECTED_BODY_MESSAGE = EXPECTED_RESPONSE + "/default_body_message";
static const std::string &EXPECTED_BODY_MESSAGE_NOT_FOUND =
	LoadFileContent(EXPECTED_BODY_MESSAGE + "/404_not_found.txt");

} // namespace test

#endif
