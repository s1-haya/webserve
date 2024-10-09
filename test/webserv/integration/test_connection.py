import json
import socket
from http import HTTPStatus
from http.client import HTTPConnection, HTTPException
from typing import Mapping, Optional, Tuple

from common_functions import SERVER_PORT, assert_response, delete_file
from common_response import (response_header_get_root_200_keep,
                             root_index_file, timeout_response)
from http_module.assert_http_response import assert_header, assert_status_line

# serverのtimeout+αを設定する
TIMEOUT = 4.0
BUFFER_SIZE = 1024

ROOT_DIR = "root/"
UPLOAD_DIR = "upload/"

CONTENT_TYPE = "Content-Type"
CONTENT_LENGTH = "Content-Length"
APPLICATION_JSON = "application/json"


def receive_with_timeout(sock) -> Optional[str]:
    # timeoutを設定
    sock.settimeout(TIMEOUT)

    # timeout秒までに受信したデータをためておく
    received_data = []
    try:
        # 一度で全てrecvできない場合用にwhile
        while True:
            data = sock.recv(BUFFER_SIZE).decode("utf-8")
            if not data:
                # 受信データを繋げてstrにして返す
                return "".join(received_data)
            received_data.append(data)
    except socket.timeout:
        # timeout秒待っても何も送られてこなかった場合
        return None


# ------------------------------------------------------------------------------
# 通常のkeep-aliveを送信(各関数の説明付き)
def test_timeout_keep_alive() -> None:
    try:
        con = HTTPConnection("localhost", SERVER_PORT)
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
            raise AssertionError
        else:
            # serverが200 OKを返したことを確認
            assert_response(
                response, response_header_get_root_200_keep + root_index_file
            )

        # 再度responseを読み込み
        next_response = receive_with_timeout(con.sock)
        if next_response is None:
            # timeout秒経過していたらNG
            raise AssertionError
        else:
            # timeout秒経過前にserverから何も送られてこなかったらOK(接続が切れたという判断はできないかも？)
            assert len(next_response) == 0

    except HTTPException as e:
        print(f"Request failed: {e}")
        raise AssertionError
    finally:
        if con:
            con.close()


def test_timeout_incomplete_request() -> None:
    try:
        con = HTTPConnection("localhost", SERVER_PORT)
        con.connect()

        # 完全ではないrequestを送信
        con.sock.send(b"GET / HTTP/1.1\r\nHost: localhost\r\nConnectio")

        response = receive_with_timeout(con.sock)
        if response is None:
            raise AssertionError
        else:
            # serverがtimeout responseを返したことを確認
            assert_response(response, timeout_response)

    except HTTPException as e:
        print(f"Request failed: {e}")
        raise AssertionError
    finally:
        if con:
            con.close()


def test_timeout_no_request() -> None:
    try:
        con = HTTPConnection("localhost", SERVER_PORT)
        con.connect()

        # requestを何も送らない

        response = receive_with_timeout(con.sock)
        if response is None:
            raise AssertionError
        else:
            # serverが何も送らず接続を切断したらOK
            assert len(response) == 0

    except HTTPException as e:
        print(f"Request failed: {e}")
        raise AssertionError
    finally:
        if con:
            con.close()


# serverがその後も正常に動いていればOK
def test_incomplete_request_disconnect() -> None:
    try:
        con = HTTPConnection("localhost", SERVER_PORT)
        con.connect()

        # 完全ではないrequestを送信
        con.sock.send(b"GET / HTTP/1.1\r\nHost: localhost\r\nConnectio")

        # 完全なリクエストを送る前に接続を切断
        con.close()

    except HTTPException as e:
        print(f"Request failed: {e}")
        raise AssertionError
    finally:
        if con:
            con.close()


# serverがその後も正常に動いていればOK
def test_timeout_no_request_disconnect() -> None:
    try:
        con = HTTPConnection("localhost", SERVER_PORT)
        con.connect()

        # requestを何も送らない

        # 何も送らずに接続を切断
        con.close()

    except HTTPException as e:
        print(f"Request failed: {e}")
        raise AssertionError
    finally:
        if con:
            con.close()


JSON_FILENAME, JSON_DATA = (
    "webserv.json",
    {
        "name": "webserv",
        "team": {
            "members": 3,
        },
    },
)
JSON_FULL_PATH = ROOT_DIR + UPLOAD_DIR + JSON_FILENAME


def create_json_request(json_data) -> Tuple[str, Mapping[str, str]]:
    def create_json_body() -> str:
        # jsonを文字列に変換し、byte列にencode
        encoded_json_data = json.dumps(json_data)
        return encoded_json_data

    # content-lengthは自動で入る
    def create_headers() -> Mapping[str, str]:
        headers = {}
        headers[CONTENT_TYPE] = APPLICATION_JSON
        return headers

    return create_json_body(), create_headers()


def test_content_type_post_json_and_get_json() -> None:
    delete_file(JSON_FULL_PATH)

    try:
        con = HTTPConnection("localhost", SERVER_PORT)

        # JSONをPOSTしてuploadしておく
        con.request(
            "POST", "/" + UPLOAD_DIR + JSON_FILENAME, *create_json_request(JSON_DATA)
        )

        response = con.getresponse()
        # 201 Created のbodyを読む
        response.read()

        # GET JSONしてContent-Typeがjsonかどうかをassert
        con.request("GET", "/" + UPLOAD_DIR + JSON_FILENAME)
        response = con.getresponse()

        assert_status_line(response, HTTPStatus.OK)
        assert_header(response, CONTENT_TYPE, APPLICATION_JSON)
        assert response.read() == json.dumps(JSON_DATA).encode("utf-8")

    except HTTPException as e:
        print(f"Request failed: {e}")
        raise AssertionError
    finally:
        if con:
            con.close()
        delete_file(JSON_FULL_PATH)
