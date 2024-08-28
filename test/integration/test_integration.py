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


def test_webserv():
    try:
        # client_module をビルド
        build_client_module()

        # client_module.client をインポート
        import client_module.client as client

        # Client クラスの使用例
        client_instance = client.Client(8080)
        response = client_instance.SendRequestAndReceiveResponse(
            "GET / HTTP/1.1\nHost: localhost\nConnection: close\r\n\r\n"
        )
        print(response)

    except subprocess.CalledProcessError as e:
        print(f"Build failed: {e}")
    except ImportError as e:
        print(f"Import failed: {e}")


if __name__ == "__main__":
    test_webserv()
