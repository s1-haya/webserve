#include <string>

namespace test {

std::string LoadFileContent(const std::string &file_path);

static const std::string EXPECTED_RESPONSE     = "../../expected_response";
static const std::string EXPECTED_STATUS_LINE  = EXPECTED_RESPONSE + "/default_status_line";
static const std::string EXPECTED_STATUS_LINE_OK =
	LoadFileContent(EXPECTED_STATUS_LINE + "/ok.txt");
static const std::string EXPECTED_STATUS_LINE_NOT_FOUND = LoadFileContent(EXPECTED_STATUS_LINE + "/not_found.txt");
static const std::string EXPECTED_BODY_MESSAGE = EXPECTED_RESPONSE + "/default_body_message";

}
