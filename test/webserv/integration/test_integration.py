import os
import subprocess

import pytest


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


def read_file(file):
    with open(file, "r") as f:
        data = f.read()
        return data, len(data)


# \r\nをそのまま読み込む用(ヘッダー部分)
def read_file_binary(file):
    with open(file, "rb") as f:
        data = f.read()
    return data, len(data)


def assert_response(response, expected_response):
    assert (
        response == expected_response
    ), f"Expected response\n\n {repr(expected_response)}, but got\n\n {repr(response)}"


def send_request_and_assert_response(request_file, expected_response):
    client_instance = client.Client(8080)
    request, _ = read_file_binary(request_file)
    response = client_instance.SendRequestAndReceiveResponse(request)
    assert_response(response, expected_response)


root_index_file, root_index_file_length = read_file("root/html/index.html")
sub_index_file, sub_index_file_length = read_file("root/html/sub/index.html")

error_files = [
    ("400_bad_request.txt", "bad_request_file_400", "bad_request_file_400_length"),
    ("404_not_found.txt", "not_found_file_404", "not_found_file_404_length"),
    (
        "405_method_not_allowed.txt",
        "not_allowed_file_405",
        "not_allowed_file_405_length",
    ),
    ("408_timeout.txt", "timeout_file_408", "timeout_file_408_length"),
    (
        "501_not_implemented.txt",
        "not_implemented_file_501",
        "not_implemented_file_501_length",
    ),
]

for filename, data_var, length_var in error_files:
    data, length = read_file_binary(
        f"test/webserv/expected_response/default_body_message/{filename}"
    )
    globals()[data_var] = data
    globals()[length_var] = length


REQUEST_DIR = "test/common/request/"
REQUEST_GET_2XX_DIR = REQUEST_DIR + "get/2xx/"
REQUEST_GET_4XX_DIR = REQUEST_DIR + "get/4xx/"
REQUEST_GET_5XX_DIR = REQUEST_DIR + "get/5xx/"

response_header_get_root_200_close = f"HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: {root_index_file_length}\r\nContent-Type: text/html\r\nServer: webserv/1.1\r\n\r\n"
response_header_get_root_200_keep = f"HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Length: {root_index_file_length}\r\nContent-Type: text/html\r\nServer: webserv/1.1\r\n\r\n"
response_header_get_sub_200_close = f"HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: {sub_index_file_length}\r\nContent-Type: text/html\r\nServer: webserv/1.1\r\n\r\n"
response_header_400 = f"HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: {bad_request_file_400_length}\r\nContent-Type: text/html\r\nServer: webserv/1.1\r\n\r\n"
response_header_404 = f"HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Length: {not_found_file_404_length}\r\nContent-Type: text/html\r\nServer: webserv/1.1\r\n\r\n"
response_header_405 = f"HTTP/1.1 405 Method Not Allowed\r\nConnection: close\r\nContent-Length: {not_allowed_file_405_length}\r\nContent-Type: text/html\r\nServer: webserv/1.1\r\n\r\n"
response_header_408 = f"HTTP/1.1 408 Request Timeout\r\nConnection: close\r\nContent-Length: {timeout_file_408_length}\r\nContent-Type: text/html\r\nServer: webserv/1.1\r\n\r\n"
response_header_501 = f"HTTP/1.1 501 Not Implemented\r\nConnection: close\r\nContent-Length: {not_implemented_file_501_length}\r\nContent-Type: text/html\r\nServer: webserv/1.1\r\n\r\n"

bad_request_response = response_header_400 + bad_request_file_400.decode("utf-8")
not_found_response = response_header_404 + not_found_file_404.decode("utf-8")
not_allowed_response = response_header_405 + not_allowed_file_405.decode("utf-8")
timeout_response = response_header_408 + timeout_file_408.decode("utf-8")
not_implemented_response = response_header_501 + not_implemented_file_501.decode(
    "utf-8"
)


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
        (REQUEST_GET_4XX_DIR + "404_01_not_exist_path.txt", not_found_response),
        (REQUEST_GET_4XX_DIR + "405_01_not_allowed.txt", not_allowed_response),
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
