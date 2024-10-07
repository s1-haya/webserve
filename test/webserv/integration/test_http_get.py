import pytest
from common_functions import send_request_and_assert_response
from common_response import (bad_request_response, not_allowed_response,
                             not_found_response, not_implemented_response,
                             response_header_get_root_200_close,
                             response_header_get_root_200_keep,
                             response_header_get_sub_200_close,
                             root_index_file, sub_index_file)

REQUEST_DIR = "test/common/request/"
REQUEST_GET_2XX_DIR = REQUEST_DIR + "get/2xx/"
REQUEST_GET_4XX_DIR = REQUEST_DIR + "get/4xx/"
REQUEST_GET_5XX_DIR = REQUEST_DIR + "get/5xx/"


@pytest.mark.parametrize(
    "request_file, expected_response",
    [
        (
            REQUEST_GET_2XX_DIR + "200_01_connection_close.txt",
            response_header_get_root_200_close + root_index_file,
        ),
        (
            REQUEST_GET_2XX_DIR + "200_02_connection_keep.txt",
            response_header_get_root_200_keep + root_index_file,
        ),
        (
            REQUEST_GET_2XX_DIR + "200_03_sub_connection_close.txt",
            response_header_get_sub_200_close + sub_index_file,
        ),
        (
            REQUEST_GET_2XX_DIR
            + "200_04_connection_keep_alive_and_200_connection_keep_alive.txt",
            response_header_get_root_200_keep
            + root_index_file
            + response_header_get_root_200_keep
            + root_index_file,
        ),
        (
            REQUEST_GET_2XX_DIR
            + "200_05_connection_close_and_200_connection_close.txt",
            response_header_get_root_200_close + root_index_file,
        ),
        (
            REQUEST_GET_2XX_DIR
            + "200_06_connection_keep_alive_and_200_connection_close.txt",
            response_header_get_root_200_keep
            + root_index_file
            + response_header_get_root_200_close
            + root_index_file,
        ),
        (
            REQUEST_GET_2XX_DIR + "200_07_no_connection_value.txt",
            response_header_get_root_200_keep + root_index_file,
        ),
        (
            REQUEST_GET_2XX_DIR + "200_08_wrong_connection_value.txt",
            response_header_get_root_200_keep + root_index_file,
        ),
        (
            REQUEST_GET_2XX_DIR + "200_11_upper_and_lower_header_fields.txt",
            response_header_get_root_200_close + root_index_file,
        ),
        (
            REQUEST_GET_2XX_DIR + "200_12_header_field_value_space.txt",
            response_header_get_root_200_close + root_index_file,
        ),
        (
            REQUEST_GET_2XX_DIR + "200_13_space_header_field_value.txt",
            response_header_get_root_200_close + root_index_file,
        ),
        (
            REQUEST_GET_2XX_DIR + "200_14_extra_request.txt",
            response_header_get_root_200_close + root_index_file,
        ),
        (
            REQUEST_GET_2XX_DIR + "200_15_body_message_default.txt",
            response_header_get_root_200_close + root_index_file,
        ),
        (
            REQUEST_GET_2XX_DIR + "200_17_not_exist_header_field.txt",
            response_header_get_root_200_close + root_index_file,
        ),
        (
            REQUEST_GET_2XX_DIR + "200_21_no_connection.txt",
            response_header_get_root_200_keep + root_index_file,
        ),
        (REQUEST_GET_4XX_DIR + "400_02_lower_method.txt", bad_request_response),
        (
            REQUEST_GET_4XX_DIR + "400_03_no_ascii_method.txt",
            bad_request_response,
        ),
        (REQUEST_GET_4XX_DIR + "400_04_no_root.txt", bad_request_response),
        (REQUEST_GET_4XX_DIR + "400_05_relative_path.txt", bad_request_response),
        (
            REQUEST_GET_4XX_DIR + "400_06_lower_http_version.txt",
            bad_request_response,
        ),
        (
            REQUEST_GET_4XX_DIR + "400_07_wrong_http_name.txt",
            bad_request_response,
        ),
        (
            REQUEST_GET_4XX_DIR + "400_08_wrong_http_version.txt",
            bad_request_response,
        ),
        (
            REQUEST_GET_4XX_DIR + "400_09_no_host.txt",
            bad_request_response,
        ),
        (REQUEST_GET_4XX_DIR + "400_10_duplicate_host.txt", bad_request_response),
        (
            REQUEST_GET_4XX_DIR + "400_11_no_header_field_colon.txt",
            bad_request_response,
        ),
        (
            REQUEST_GET_4XX_DIR + "400_12_no_connection_name.txt",
            bad_request_response,
        ),
        (
            REQUEST_GET_4XX_DIR + "400_13_empty_host_header_value.txt",
            bad_request_response,
        ),
        (
            REQUEST_GET_4XX_DIR + "400_14_transfer_encoding_and_content_length.txt",
            bad_request_response,
        ),
        (
            REQUEST_GET_4XX_DIR + "400_15_space_in_header_field_name.txt",
            bad_request_response,
        ),
        (
            REQUEST_GET_4XX_DIR + "400_16_header_field_name_space_colon.txt",
            bad_request_response,
        ),
        (
            REQUEST_GET_4XX_DIR + "400_17_space_header_field_name.txt",
            bad_request_response,
        ),
        (
            REQUEST_GET_4XX_DIR + "400_18_non_vchr_header_field_name.txt",
            bad_request_response,
        ),
        (
            REQUEST_GET_4XX_DIR + "400_19_non_vchr_header_field_value.txt",
            bad_request_response,
        ),
        (
            REQUEST_GET_4XX_DIR + "400_20_too_few_status_line_elements.txt",
            bad_request_response,
        ),
        (
            REQUEST_GET_4XX_DIR + "400_21_too_many_status_line_elements.txt",
            bad_request_response,
        ),
        (
            REQUEST_GET_4XX_DIR + "400_22_non_vchr_method.txt",
            bad_request_response,
        ),
        (
            REQUEST_GET_4XX_DIR + "400_23_non_vchr_request_target.txt",
            bad_request_response,
        ),
        (
            REQUEST_GET_4XX_DIR + "400_24_non_vchr_http_version.txt",
            bad_request_response,
        ),
        (REQUEST_GET_4XX_DIR + "404_01_not_exist_path.txt", not_found_response),
        (
            REQUEST_GET_4XX_DIR + "405_01_method_not_allowed_for_uri.txt",
            not_allowed_response,
        ),
        (
            REQUEST_GET_5XX_DIR + "501_01_not_exist_method.txt",
            not_implemented_response,
        ),
    ],
    ids=[
        "200_01_connection_close",
        "200_02_connection_keep",
        "200_03_sub_connection_close",
        "200_04_connection_keep_alive_and_200_connection_keep_alive",
        "200_05_connection_close_and_200_connection_close",
        "200_06_connection_keep_alive_and_200_connection_close",
        "200_07_no_connection_value",
        "200_08_wrong_connection_value",
        "200_11_upper_and_lower_header_fields",
        "200_12_header_field_value_space",
        "200_13_space_header_field_value",
        "200_14_extra_request",
        "200_15_body_message_default",
        "200_17_not_exist_header_field",
        "200_21_no_connection",
        "400_02_lower_method",
        "400_03_no_ascii_method",
        "400_04_no_root",
        "400_05_relative_path",
        "400_06_lower_http_version",
        "400_07_wrong_http_name",
        "400_08_wrong_http_version",
        "400_09_no_host",
        "400_10_duplicate_host",
        "400_11_no_header_field_colon",
        "400_12_no_connection_name",
        "400_13_empty_host_header_value",
        "400_14_transfer_encoding_and_content_length.txt",
        "400_15_space_in_header_field_name",
        "400_16_header_field_name_space_colon",
        "400_17_space_header_field_name",
        "400_18_non_vchr_header_field_name",
        "400_19_non_vchr_header_field_value",
        "400_20_too_few_status_line_elements",
        "400_21_too_many_status_line_elements",
        "400_22_non_vchr_method",
        "400_23_non_vchr_request_target",
        "400_24_non_vchr_http_version",
        "404_01_not_exist_path",
        "405_01_method_not_allowed_for_uri",
        "501_01_not_exist_method",
    ],
)
def test_get_responses(request_file, expected_response):
    send_request_and_assert_response(request_file, expected_response)
