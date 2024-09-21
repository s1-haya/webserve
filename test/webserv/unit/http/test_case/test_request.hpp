#include <string>

namespace test {

std::string LoadFileContent(const std::string &file_path);

static const std::string &COMMON = "../../../common";
static const std::string &REQUEST = COMMON + "/request";
static const std::string &REQUEST_GET = REQUEST + "/get";

static const std::string &NOT_EXIST_PATH_CONNECTION_CLOSE =
	test::LoadFileContent(
		REQUEST_GET + "/404_not-exist-path_connection-close.txt"
	);
}
