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


# def test_webserv():
#     try:
#         test1()
#         test2()
#         test3()
#         test4()
#         test5()
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
