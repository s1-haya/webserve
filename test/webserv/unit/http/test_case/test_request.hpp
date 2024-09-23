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
static const std::string &GET_200_1_NO_CONNECTION =
	LoadFileContent(REQUEST_GET + ROOT_2XX + "/200_01_connection_close.txt");
static const std::string &GET_200_13_EXTRA_REQUEST =
	LoadFileContent(REQUEST_GET + ROOT_2XX + "/200_13_extra_request.txt");

static const std::string &ROOT_4XX = "/4xx";
static const std::string &GET_400_1_ONLY_CRLF =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_01_only_crlf.txt");
static const std::string &GET_404_1_NOT_EXIST_PATH_CONNECTION_CLOSE =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/404_01_not_exist_path.txt");
static const std::string &GET_405_1_NOT_ALLOWED =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/405_01_not_allowed.txt");
static const std::string &GET_408_1_NO_CRLF =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/408_01_no_crlf.txt");

static const std::string &ROOT_5XX = "/5xx";
static const std::string &GET_501_1_NOT_EXIST_METHOD =
	LoadFileContent(REQUEST_GET + ROOT_5XX + "/501_01_not_exist_method.txt");

} // namespace request

} // namespace test

#endif
