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
	test::LoadFileContent(REQUEST_GET + ROOT_2XX + "/200_01_no_connection.txt");
static const std::string &REQUEST_GET_404_1_NOT_EXIST_PATH_CONNECTION_CLOSE =
	test::LoadFileContent(REQUEST_GET + ROOT_4XX + "/404_01_not_exist_path.txt");

} // namespace request

} // namespace test

#endif
