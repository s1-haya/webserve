import socket
from http.client import HTTPConnection, HTTPException
from typing import Optional

from common import (assert_response, response_header_get_root_200_keep,
                    root_index_file, timeout_response)

# serverのtimeout+αを設定する
TIMEOUT = 4.0
BUFFER_SIZE = 10240


def receive_with_timeout(sock) -> Optional[str]:
    # timeoutを設定
    sock.settimeout(TIMEOUT)
    try:
        # timeout秒までに何かを受信したらその時点で返す
        return sock.recv(BUFFER_SIZE).decode("utf-8")
    except socket.timeout:
        # timeout秒待っても何も送られてこなかった場合
        return None


# ------------------------------------------------------------------------------
# 通常のkeep-aliveを送信(各関数の説明付き)
def test_timeout_keep_alive() -> None:
    try:
        con = HTTPConnection("localhost", 8080)
        # serverに接続
        con.connect()

        # request送信
        con.sock.send(
            b"GET / HTTP/1.1\r\nHost: localhost\r\nConnection: keep-alive\r\n\r\n"
        )

        # serverからのresponseを読み込む
        response = receive_with_timeout(con.sock)
        if response is None:
            # timeout秒経過していたらNG
            assert False
        else:
            # serverが200 OKを返したことを確認
            assert_response(
                response, response_header_get_root_200_keep + root_index_file
            )

        # 再度responseを読み込み
        next_response = receive_with_timeout(con.sock)
        if next_response is None:
            # timeout秒経過していたらNG
            assert False
        else:
            # timeout秒経過前にserverから何も送られてこなかったらOK(接続が切れたという判断はできないかも？)
            assert len(next_response) == 0

    except HTTPException as e:
        print(f"Request failed: {e}")
        assert False
    finally:
        if con:
            con.close()


def test_timeout_incomplete_request() -> None:
    try:
        con = HTTPConnection("localhost", 8080)
        con.connect()

        # 完全ではないrequestを送信
        con.sock.send(b"GET / HTTP/1.1\r\nHost: localhost\r\nConnectio")

        response = receive_with_timeout(con.sock)
        if response is None:
            assert False
        else:
            # serverがtimeout responseを返したことを確認
            assert_response(response, timeout_response)

    except HTTPException as e:
        print(f"Request failed: {e}")
        assert False
    finally:
        if con:
            con.close()


def test_timeout_no_request() -> None:
    try:
        con = HTTPConnection("localhost", 8080)
        con.connect()

        # requestを何も送らない

        response = receive_with_timeout(con.sock)
        if response is None:
            assert False
        else:
            # serverが何も送らず接続を切断したらOK
            assert len(response) == 0

    except HTTPException as e:
        print(f"Request failed: {e}")
        assert False
    finally:
        if con:
            con.close()


# serverがその後も正常に動いていればOK
def test_incomplete_request_disconnect() -> None:
    try:
        con = HTTPConnection("localhost", 8080)
        con.connect()

        # 完全ではないrequestを送信
        con.sock.send(b"GET / HTTP/1.1\r\nHost: localhost\r\nConnectio")

        # 完全なリクエストを送る前に接続を切断
        con.close()

    except HTTPException as e:
        print(f"Request failed: {e}")
        assert False
    finally:
        if con:
            con.close()


# serverがその後も正常に動いていればOK
def test_timeout_no_request_disconnect() -> None:
    try:
        con = HTTPConnection("localhost", 8080)
        con.connect()

        # requestを何も送らない

        # 何も送らずに接続を切断
        con.close()

    except HTTPException as e:
        print(f"Request failed: {e}")
        assert False
    finally:
        if con:
            con.close()
