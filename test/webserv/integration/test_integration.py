import os
import subprocess

import pytest
from common import *


def build_client_module():
    # client_module ディレクトリに移動
    client_module_dir = os.path.join(os.path.dirname(__file__), "client_module")
    # コマンドを実行
    subprocess.run(
        ["python3", "setup.py", "build_ext", "--inplace"],
        cwd=client_module_dir,
        check=True,
    )


try:
    # client_module をビルド
    build_client_module()
    # client_module.client をインポート
    import client_module.client as client
except subprocess.CalledProcessError as e:
    print(f"Build failed: {e}")
except ImportError as e:
    print(f"Import failed: {e}")


def send_request_and_assert_response(request_file, expected_response):
    client_instance = client.Client(8080)
    request, _ = read_file_binary(request_file)
    response = client_instance.SendRequestAndReceiveResponse(request)
    assert_response(response, expected_response)


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
)
def test_get_responses(request_file, expected_response):
    send_request_and_assert_response(request_file, expected_response)


# >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# def test_webserv():
#     try:
#         test_get_root_close_200()
#         test_get_root_keep_200()
#         test_get_sub_close_200()
#         test_get_404()
#         test_get_405()
#     except Exception as e:
#         print(f"Test failed: {e}")


# def test1():
#     client_instance = client.Client(8080)
#     response = client_instance.SendRequestAndReceiveResponse("")
#     print(response)
#     # assert


# def test2():
#     client_instance = client.Client(8080)
#     response = client_instance.SendRequestAndReceiveResponse("GET / HTTP/1.1\n")
#     print(response)
#     # assert


# def test3():
#     client_instance = client.Client(8080)
#     response = client_instance.SendRequestAndReceiveResponse("aaa" * 10000)
#     print(response)
#     # assert


# def test4():
#     client_instance = client.Client(8080)
#     while True:
#         response = client_instance.SendRequestAndReceiveResponse(
#             "GET / HTTP/1.1\nHost: localhost\r\n\r\n"
#         )
#     print(response)
#     # assert


# def test5():
#     client_instance = client.Client(8080)
#     response = client_instance.SendRequestAndReceiveResponse(
#         "GET \n   Test: aaa  \r\n\r\n"
#     )
#     print(response)
#     # assert


# if __name__ == "__main__":
#     test_webserv()

# <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
