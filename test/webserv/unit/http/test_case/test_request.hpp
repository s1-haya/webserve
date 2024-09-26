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

static const std::string &ROOT_4XX = "/4xx";
static const std::string &GET_400_1_ONLY_CRLF =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_01_only_crlf.txt");
static const std::string &GET_400_2_LOWER_METHOD =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_02_lower_method.txt");
static const std::string &GET_400_3_NO_ASCII_METHOD =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_03_no_ascii_method.txt");
static const std::string &GET_400_4_NO_ROOT =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_04_no_root.txt");
static const std::string &GET_400_5_RELATIVE_PATH =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_05_relative_path.txt");
static const std::string &GET_400_6_LOWER_HTTP_VERSION =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_06_lower_http_version.txt");
static const std::string &GET_400_7_WRONG_HTTP_NAME =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_07_wrong_http_name.txt");
static const std::string &GET_400_8_WRONG_HTTP_VERSION =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_08_wrong_http_version.txt");
static const std::string &GET_400_9_NO_HOST =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_09_no_host.txt");
static const std::string &GET_400_10_DUPLICATE_HOST =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_10_duplicate_host.txt");
static const std::string &GET_400_11_NO_HEADER_FIELD_COLON =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_11_no_header_field_colon.txt");
static const std::string &GET_400_12_NO_CONNECTION_NAME =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_12_no_connection_name.txt");
static const std::string &GET_400_13_NO_CONNECTION_VALUE =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_13_no_connection_value.txt");
static const std::string &GET_400_14_WRONG_CONNECTION_VALUE =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_14_wrong_connection_value.txt");
static const std::string &GET_400_15_NOT_EXIST_HEADER_FIELD =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_15_not_exist_header_field.txt");
static const std::string &GET_400_16_HEADER_FIELD_NAME_SPACE_COLON =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_16_header_field_name_space_colon.txt");
static const std::string &GET_400_17_SPACE_HEADER_FIELD_NAME =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_17_space_header_field_name.txt");

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
