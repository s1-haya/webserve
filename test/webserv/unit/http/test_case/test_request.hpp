#ifndef TEST_REQUEST_HPP_
#define TEST_REQUEST_HPP_

#include <string>

namespace test {

std::string LoadFileContent(const std::string &file_path);

namespace request {

static const std::string &COMMON      = "../../../common";
static const std::string &REQUEST     = COMMON + "/request";
static const std::string &REQUEST_GET = REQUEST + "/get";

static const std::string &ROOT_2XX = "/2xx";
static const std::string &ROOT_4XX = "/4xx";

static const std::string &REQUEST_GET_200_1_NO_CONNECTION =
	LoadFileContent(REQUEST_GET + ROOT_2XX + "/200_01_connection_close.txt");

static const std::string &GET_400_1_ONLY_CRLF = LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_01_only_crlf.txt");
static const std::string &GET_408_1_NO_CRLF = LoadFileContent(REQUEST_GET + ROOT_4XX + "/408_01_no_crlf.txt");
static const std::string &REQUEST_GET_404_1_NOT_EXIST_PATH_CONNECTION_CLOSE =
		LoadFileContent(REQUEST_GET + ROOT_4XX + "/404_01_not_exist_path.txt");

} // namespace request

} // namespace test

#endif
