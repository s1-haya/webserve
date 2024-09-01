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
        return f.read()


# \r\nをそのまま読み込む用(ヘッダー部分)
def read_file_binary(file):
    with open(file, "rb") as f:
        return f.read()


response_header_get_root_200 = (
    "HTTP/1.1 200 OK\r\nConnection: close \r\nContent-Length: 1250 \r\n\r\n"
)

response_header_get_sub_200 = (
    "HTTP/1.1 200 OK\r\nConnection: close \r\nContent-Length: 398 \r\n\r\n"
)


def test_get_root_close_200():
    expected_response = response_header_get_root_200 + read_file(
        "../../html/index.html"
    )
    client_instance = client.Client(8080)
    request = read_file_binary(
        "../request_messages/webserv/get/200_root_connection-close.txt"
    )
    response = client_instance.SendRequestAndReceiveResponse(request)
    assert (
        response == expected_response
    ), f"Expected response\n\n {repr(expected_response)}, but got\n\n {repr(response)}"


def test_get_root_keep_200():
    expected_response = response_header_get_root_200 + read_file(
        "../../html/index.html"
    )
    # responseヘッダーもkeepaliveになる？
    client_instance = client.Client(8080)
    request = read_file_binary(
        "../request_messages/webserv/get/200_root_connection-keep.txt"
    )
    response = client_instance.SendRequestAndReceiveResponse(request)
    assert (
        response == expected_response
    ), f"Expected response\n\n {repr(expected_response)}, but got\n\n {repr(response)}"


def test_get_sub_close_200():
    expected_response = response_header_get_sub_200 + read_file(
        "../../html/sub/index.html"
    )
    client_instance = client.Client(8080)
    request = read_file_binary(
        "../request_messages/webserv/get/200_sub_connection-close.txt"
    )
    response = client_instance.SendRequestAndReceiveResponse(request)
    assert (
        response == expected_response
    ), f"Expected response\n\n {repr(expected_response)}, but got\n\n {repr(response)}"


# def test_get_404():
#     expected_response = response_header_get_404 + read_file("../../html/sub/index.html")
#     client_instance = client.Client(8080)
#     request = read_file_binary("../request_messages/webserv/get/404_not-exist-path_connection-close.txt")
#     response = client_instance.SendRequestAndReceiveResponse(request)
#     assert (
#         response == expected_response
#     ), f"Expected response\n\n {repr(expected_response)}, but got\n\n {repr(response)}"


def test_webserv():
    try:
        test_get_root_close_200()
        test_get_root_keep_200()
        test_get_sub_close_200()
        # test_get_404()
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
