#ifndef TEST_REQUEST_HPP_
#define TEST_REQUEST_HPP_

#include <string>

namespace test {

std::string LoadFileContent(const std::string &file_path);

namespace request {

static const std::string &COMMON  = "../../../common";
static const std::string &REQUEST = COMMON + "/request";

static const std::string &ROOT_2XX = "/2xx";
static const std::string &ROOT_4XX = "/4xx";
static const std::string &ROOT_5XX = "/5xx";

static const std::string &REQUEST_GET = REQUEST + "/get";

static const std::string &GET_200_1_CONNECTION_CLOSE =
	LoadFileContent(REQUEST_GET + ROOT_2XX + "/200_01_connection_close.txt");
static const std::string &GET_200_2_CONNECTION_KEEP =
	LoadFileContent(REQUEST_GET + ROOT_2XX + "/200_02_connection_keep.txt");
static const std::string &GET_200_3_SUB_CONNECTION_CLOSE =
	LoadFileContent(REQUEST_GET + ROOT_2XX + "/200_03_sub_connection_close.txt");
static const std::string &GET_200_4_CONNECTION_KEEP_AND_200_CONNECTION_KEEP = LoadFileContent(
	REQUEST_GET + ROOT_2XX + "/200_04_connection_keep_alive_and_200_connection_keep_alive.txt"
);
static const std::string &GET_200_5_CONNECTION_CLOSE_AND_200_CONNECTION_CLOSE = LoadFileContent(
	REQUEST_GET + ROOT_2XX + "/200_05_connection_close_and_200_connection_close.txt"
);
static const std::string &GET_200_6_CONNECTION_KEEP_AND_200_CONNECTION_CLOSE = LoadFileContent(
	REQUEST_GET + ROOT_2XX + "/200_06_connection_keep_alive_and_200_connection_close.txt"
);
static const std::string &GET_200_11_UPPER_AND_LOWER_HEADER_FIELDS =
	LoadFileContent(REQUEST_GET + ROOT_2XX + "/200_11_upper_and_lower_header_fields.txt");
static const std::string &GET_200_12_HEADER_FIELD_VALUE_SPACE =
	LoadFileContent(REQUEST_GET + ROOT_2XX + "/200_12_header_field_value_space.txt");
static const std::string &GET_200_13_SPACE_HEADER_FIELD_VALUE =
	LoadFileContent(REQUEST_GET + ROOT_2XX + "/200_13_space_header_field_value.txt");
static const std::string &GET_200_14_EXTRA_REQUEST =
	LoadFileContent(REQUEST_GET + ROOT_2XX + "/200_14_extra_request.txt");
static const std::string &GET_200_15_BODY_MESSAGE_DEFAULT =
	LoadFileContent(REQUEST_GET + ROOT_2XX + "/200_15_body_message_default.txt");
static const std::string &GET_200_17_NOT_EXIST_HEADER_FIELD =
	LoadFileContent(REQUEST_GET + ROOT_2XX + "/200_17_not_exist_header_field.txt");
static const std::string &GET_200_21_NO_CONNECTION =
	LoadFileContent(REQUEST_GET + ROOT_2XX + "/200_21_no_connection.txt");

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
static const std::string &GET_400_15_SPACE_IN_HEADER_FIELD_NAME =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_15_space_in_header_field_name.txt");
static const std::string &GET_400_16_HEADER_FIELD_NAME_SPACE_COLON =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_16_header_field_name_space_colon.txt");
static const std::string &GET_400_17_SPACE_HEADER_FIELD_NAME =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_17_space_header_field_name.txt");
static const std::string &GET_400_18_NON_VCHR_HEADER_FIELD_NAME =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_18_non_vchr_header_field_name.txt");
static const std::string &GET_400_19_NON_VCHR_HEADER_FIELD_VALUE =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/400_19_non_vchr_header_field_value.txt");
static const std::string &GET_404_1_NOT_EXIST_PATH_CONNECTION_CLOSE =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/404_01_not_exist_path.txt");
static const std::string &GET_405_1_NOT_ALLOWED =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/405_01_method_not_allowed_for_uri.txt");
static const std::string &GET_408_1_NO_CRLF =
	LoadFileContent(REQUEST_GET + ROOT_4XX + "/408_01_no_crlf.txt");

static const std::string &GET_501_1_NOT_EXIST_METHOD =
	LoadFileContent(REQUEST_GET + ROOT_5XX + "/501_01_not_exist_method.txt");

static const std::string &REQUEST_DELETE = REQUEST + "/delete";

static const std::string &DELETE_204_1_EXISTING_FILE =
	LoadFileContent(REQUEST_DELETE + ROOT_2XX + "/204_01_delete_existing_file.txt");
static const std::string &DELETE_204_2_EXISTING_FILE_WITH_BODY_MESSAGE = LoadFileContent(
	REQUEST_DELETE + ROOT_2XX + "/204_02_delete_existing_file_with_body_message.txt"
);
static const std::string &DELETE_204_3_EXISTING_FILE_THEN_404_ON_SECOND_ATTEMPT = LoadFileContent(
	REQUEST_DELETE + ROOT_2XX + "/204_03_delete_existing_file_then_404_on_second_attempt.txt"
);
static const std::string &DELETE_403_1_DIRECTORY =
	LoadFileContent(REQUEST_DELETE + ROOT_4XX + "/403_01_delete_directory.txt");
static const std::string &DELETE_404_1_NONEXISTENT_FILE =
	LoadFileContent(REQUEST_DELETE + ROOT_4XX + "/404_01_delete_nonexistent_file.txt");
static const std::string &DELETE_405_NOT_ALLOWED =
	LoadFileContent(REQUEST_DELETE + ROOT_4XX + "/405_01_method_not_allowed_for_uri.txt");

namespace cgi {

static const std::string &REQUEST_CGI = REQUEST + "/cgi";
static const std::string &CGI_GET     = REQUEST_CGI + "/get";

static const std::string &GET_200_01_PRINT_OK_PL_BODY_MESSAGE =
	LoadFileContent(CGI_GET + "/200_01_print_ok_pl_body_message.txt");
static const std::string &GET_200_02_PRINT_OK_PERL =
	LoadFileContent(CGI_GET + "/200_02_print_ok_pl.txt");
static const std::string &GET_200_03_PRINT_OK_PYTHON =
	LoadFileContent(CGI_GET + "/200_03_print_ok_py.txt");
static const std::string &GET_200_04_PRINT_OK_SHELL =
	LoadFileContent(CGI_GET + "/200_04_print_ok_sh.txt");

} // namespace cgi
} // namespace request
} // namespace test

#endif
