import os
import subprocess


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


root_index_file, root_index_file_length = read_file("root/html/index.html")
sub_index_file, sub_index_file_length = read_file("root/html/sub/index.html")

# bad_request_file_400, bad_request_file_400_length = read_file_binary("test/webserv/expected_response/default_body_message/400_bad_request.txt")
not_found_file_404, not_found_file_404_length = read_file_binary("test/webserv/expected_response/default_body_message/404_not_found.txt")
not_allowed_file_405, not_allowed_file_405_length = read_file_binary("test/webserv/expected_response/default_body_message/405_method_not_allowed.txt")

response_header_get_root_200_close = f"HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: {root_index_file_length}\r\nContent-Type: text/html\r\nServer: webserv/1.1\r\n\r\n"
response_header_get_root_200_keep = f"HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Length: {root_index_file_length}\r\nContent-Type: text/html\r\nServer: webserv/1.1\r\n\r\n"
response_header_get_sub_200_close = f"HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Length: {sub_index_file_length}\r\nContent-Type: text/html\r\nServer: webserv/1.1\r\n\r\n"
# response_header_get_400 = f"HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-Length: {bad_request_file_400_length}\r\nContent-Type: text/html\r\nServer: webserv/1.1\r\n\r\n"
response_header_get_404 = f"HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-Length: {not_found_file_404_length}\r\nContent-Type: text/html\r\nServer: webserv/1.1\r\n\r\n"
response_header_get_405 = f"HTTP/1.1 405 Method Not Allowed\r\nConnection: close\r\nContent-Length: {not_allowed_file_405_length}\r\nContent-Type: text/html\r\nServer: webserv/1.1\r\n\r\n"

def test_get_root_close_200():
    expected_response = response_header_get_root_200_close + root_index_file
    client_instance = client.Client(8080)
    request, _ = read_file_binary(
        "test/common/request/get/2xx/200_01_connection_close.txt"
    )
    response = client_instance.SendRequestAndReceiveResponse(request)
    assert_response(response, expected_response)


def test_get_root_keep_200():
    expected_response = response_header_get_root_200_keep + root_index_file
    # responseヘッダーもkeepaliveになる？
    client_instance = client.Client(8080)
    request, _ = read_file_binary("test/common/request/get/2xx/200_02_connection_keep.txt")
    response = client_instance.SendRequestAndReceiveResponse(request)
    assert_response(response, expected_response)


def test_get_sub_close_200():
    expected_response = response_header_get_sub_200_close + sub_index_file
    client_instance = client.Client(8080)
    request, _ = read_file_binary(
        "test/common/request/get/2xx/200_03_sub_connection_close.txt"
    )
    response = client_instance.SendRequestAndReceiveResponse(request)
    assert_response(response, expected_response)


# def test_get_400():
#     expected_response = response_header_get_400.encode('utf-8') + bad_request_file_400
#     client_instance = client.Client(8080)
#     request, _ = read_file_binary("test/common/request/get/4xx/400_01_only_crlf.txt")
#     response = client_instance.SendRequestAndReceiveResponse(request)
#     assert (
#         response.encode('utf-8') == expected_response
#     ), f"Expected response\n\n {repr(expected_response)}, but got\n\n {repr(response)}"


def test_get_404():
    expected_response = response_header_get_404.encode('utf-8') + not_found_file_404
    client_instance = client.Client(8080)
    request, _ = read_file_binary("test/common/request/get/4xx/404_01_not_exist_path.txt")
    response = client_instance.SendRequestAndReceiveResponse(request)
    assert (
        response.encode('utf-8') == expected_response
    ), f"Expected response\n\n {repr(expected_response)}, but got\n\n {repr(response)}"


def test_get_405():
    expected_response = response_header_get_405.encode('utf-8') + not_allowed_file_405
    client_instance = client.Client(8080)
    request, _ = read_file_binary("test/common/request/get/4xx/405_01_not_allowed.txt")
    response = client_instance.SendRequestAndReceiveResponse(request)
    assert (
        response.encode('utf-8') == expected_response
    ), f"Expected response\n\n {repr(expected_response)}, but got\n\n {repr(response)}"


def test_webserv():
    try:
        test_get_root_close_200()
        test_get_root_keep_200()
        test_get_sub_close_200()
        test_get_404()
        test_get_405()
    except Exception as e:
        print(f"Test failed: {e}")


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


if __name__ == "__main__":
    test_webserv()
