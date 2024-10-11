import unittest
from http import HTTPStatus
from http.client import HTTPConnection, HTTPException, HTTPResponse

from common_functions import SERVER_PORT
from http_module.assert_http_response import (assert_body, assert_header,
                                              assert_status_line)

NOT_FOUND_FILE_PATH = (
    "test/webserv/expected_response/default_body_message/404_not_found.txt"
)
METHOD_NOT_ALLOWED_FILE_PATH = (
    "test/webserv/expected_response/default_body_message/405_method_not_allowed.txt"
)
TIMEOUT_FILE_PATH = (
    "test/webserv/expected_response/default_body_message/408_timeout.txt"
)
INTERNAL_SERVER_ERROR_FILE_PATH = (
    "test/webserv/expected_response/default_body_message/500_internal_server_error.txt"
)


def assert_body_binary(response: HTTPResponse, path: str) -> None:
    response_body = response.read()
    with open(path, "rb") as f:
        expected_body = f.read()
    assert (
        response_body == expected_body
    ), f"Expected response body (binary) {expected_body}, but got {response_body}"
    assert_header(response, "Content-Length", str(len(expected_body)))


class TestCGI(unittest.TestCase):
    def setUp(self):
        # 各テストの前に実行される(unittestの機能)
        self.con = HTTPConnection("localhost", SERVER_PORT)

    def tearDown(self):
        # 各テストの後に実行される(unittestの機能)
        self.con.close()

    def test_print_ok_pl_close(self):
        try:
            self.con.request(
                "GET", "/cgi-bin/print_ok.pl", headers={"Connection": "close"}
            )
            response = self.con.getresponse()
            assert_status_line(response, HTTPStatus.OK)
            assert_header(response, "Connection", "close")
            self.assertEqual(response.read(), b"OK\n")
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    def test_print_ok_pl_keep(self):
        try:
            self.con.request(
                "GET", "/cgi-bin/print_ok.pl", headers={"Connection": "keep-alive"}
            )
            response = self.con.getresponse()
            assert_status_line(response, HTTPStatus.OK)
            assert_header(response, "Connection", "keep-alive")
            self.assertEqual(response.read(), b"OK\n")
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    # CgiResponseの中のConnectionヘッダーが適用されているか
    def test_print_ok_close_pl(self):
        try:
            self.con.request(
                "GET",
                "/cgi-bin/print_ok_close.pl",
                headers={"Connection": "keep-alive"},
            )
            response = self.con.getresponse()
            assert_status_line(response, HTTPStatus.OK)
            assert_header(response, "Connection", "close")
            self.assertEqual(response.read(), b"OK\n")
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    def test_print_ok_keep_pl(self):
        try:
            self.con.request(
                "GET", "/cgi-bin/print_ok_keep.pl", headers={"Connection": "close"}
            )
            response = self.con.getresponse()
            assert_status_line(response, HTTPStatus.OK)
            assert_header(response, "Connection", "keep-alive")
            self.assertEqual(response.read(), b"OK\n")
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    def test_print_env_pl(self):
        try:
            self.con.request("GET", "/cgi-bin/print_env.pl")
            response = self.con.getresponse()
            assert_status_line(response, HTTPStatus.OK)
            assert_header(response, "Connection", "keep-alive")
            body = response.read().decode()
            self.assertIn("AUTH_TYPE:", body)
            self.assertIn("CONTENT_LENGTH:", body)
            self.assertIn("CONTENT_TYPE:", body)
            self.assertIn("GATEWAY_INTERFACE:", body)
            self.assertIn("PATH_INFO:", body)
            self.assertIn("PATH_TRANSLATED:", body)
            self.assertIn("QUERY_STRING:", body)
            self.assertIn("REMOTE_ADDR:", body)
            self.assertIn("REMOTE_HOST:", body)
            self.assertIn("REMOTE_IDENT:", body)
            self.assertIn("REMOTE_USER:", body)
            self.assertIn("REQUEST_METHOD:", body)
            self.assertIn("SCRIPT_NAME:", body)
            self.assertIn("SERVER_NAME:", body)
            self.assertIn("SERVER_PORT:", body)
            self.assertIn("SERVER_PROTOCOL:", body)
            self.assertIn("SERVER_SOFTWARE:", body)
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    def test_print_ok_py(self):
        try:
            self.con.request("GET", "/cgi-bin/print_ok.py")
            response = self.con.getresponse()
            assert_status_line(response, HTTPStatus.OK)
            assert_header(response, "Connection", "keep-alive")
            # ファイルが実行されずに中身が返ってくればOK
            assert_body(response, "root/cgi-bin/print_ok.py")
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    def test_print_stdin_pl(self):
        try:
            headers = {"Content-Type": "application/x-www-form-urlencoded"}
            body = "key1=value1&key2=value2"
            self.con.request("POST", "/cgi-bin/print_stdin.pl", body, headers)
            response = self.con.getresponse()
            assert_status_line(response, HTTPStatus.OK)
            assert_header(response, "Connection", "keep-alive")
            # POSTしたデータが返ってくればOK
            self.assertIn(b"POST data: key1=value1&key2=value2", response.read())
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    def test_print_stdin_pl_no_data(self):
        try:
            headers = {"Content-Type": "application/x-www-form-urlencoded"}
            self.con.request("POST", "/cgi-bin/print_stdin.pl", "", headers)
            response = self.con.getresponse()
            assert_status_line(response, HTTPStatus.OK)
            assert_header(response, "Connection", "keep-alive")
            # POSTデータがない場合
            self.assertIn(b"No POST data received.", response.read())
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    def test_path_info_pl(self):
        try:
            self.con.request("GET", "/cgi-bin/path_info.pl/path_info")
            response = self.con.getresponse()
            assert_status_line(response, HTTPStatus.OK)
            assert_header(response, "Connection", "keep-alive")
            self.assertIn(b"PathInfo string: /path_info\n", response.read())
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    def test_client_redirect_pl(self):
        try:
            self.con.request("GET", "/cgi-bin/client_redirect.pl")
            response = self.con.getresponse()
            assert_status_line(response, HTTPStatus.FOUND)
            assert_header(response, "Connection", "keep-alive")
            assert_header(response, "Location", "http://localhost:8080/")
            # bodyは空
            self.assertEqual(response.read().decode(), "")
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    def test_client_redirect_with_doc_pl(self):
        try:
            self.con.request("GET", "/cgi-bin/client_redirect_with_doc.pl")
            response = self.con.getresponse()
            assert_status_line(response, HTTPStatus.FOUND)
            assert_header(response, "Connection", "keep-alive")
            assert_header(response, "Location", "http://localhost:8080/")
            expected_body = (
                "<html>\n"
                "<head><title>Redirect</title></head>\n"
                "<body>\n"
                '<p>Redirecting to <a href="http://localhost:8080/">http://localhost:8080/</a></p>\n'
                "</body>\n"
                "</html>\n"
            )
            self.assertEqual(response.read().decode(), expected_body)
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    def test_local_redirect_pl(self):
        try:
            self.con.request("GET", "/cgi-bin/local_redirect.pl")
            response = self.con.getresponse()
            # test_print_env_plと同じ内容が返ってくるはず
            assert_status_line(response, HTTPStatus.OK)
            assert_header(response, "Connection", "keep-alive")
            body = response.read().decode()
            self.assertIn("AUTH_TYPE:", body)
            self.assertIn("CONTENT_LENGTH:", body)
            self.assertIn("CONTENT_TYPE:", body)
            self.assertIn("GATEWAY_INTERFACE:", body)
            self.assertIn("PATH_INFO:", body)
            self.assertIn("PATH_TRANSLATED:", body)
            self.assertIn("QUERY_STRING:", body)
            self.assertIn("REMOTE_ADDR:", body)
            self.assertIn("REMOTE_HOST:", body)
            self.assertIn("REMOTE_IDENT:", body)
            self.assertIn("REMOTE_USER:", body)
            self.assertIn("REQUEST_METHOD:", body)
            self.assertIn("SCRIPT_NAME:", body)
            self.assertIn("SERVER_NAME:", body)
            self.assertIn("SERVER_PORT:", body)
            self.assertIn("SERVER_PROTOCOL:", body)
            self.assertIn("SERVER_SOFTWARE:", body)
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    def test_loop_local_redirect_pl(self):
        try:
            self.con.request("GET", "/cgi-bin/loop_local_redirect.pl")
            response = self.con.getresponse()
            # 無限ループしてtimeoutへ
            assert_status_line(response, HTTPStatus.REQUEST_TIMEOUT)
            assert_header(response, "Connection", "close")
            assert_body_binary(response, TIMEOUT_FILE_PATH)
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    def test_no_header_pl(self):
        try:
            self.con.request("GET", "/cgi-bin/error/no_header.pl")
            response = self.con.getresponse()
            self.assertEqual(response.status, HTTPStatus.INTERNAL_SERVER_ERROR)
            assert_header(response, "Connection", "close")
            assert_body_binary(response, INTERNAL_SERVER_ERROR_FILE_PATH)
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    def test_not_executable_pl(self):
        try:
            self.con.request("GET", "/cgi-bin/error/not_executable.pl")
            response = self.con.getresponse()
            self.assertEqual(response.status, HTTPStatus.INTERNAL_SERVER_ERROR)
            assert_header(response, "Connection", "close")
            assert_body_binary(response, INTERNAL_SERVER_ERROR_FILE_PATH)
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    def test_invalid_header_pl(self):
        try:
            self.con.request("GET", "/cgi-bin/error/invalid_header.pl")
            response = self.con.getresponse()
            self.assertEqual(response.status, HTTPStatus.INTERNAL_SERVER_ERROR)
            assert_header(response, "Connection", "close")
            assert_body_binary(response, INTERNAL_SERVER_ERROR_FILE_PATH)
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    def test_loop_pl(self):
        try:
            self.con.request("GET", "/cgi-bin/loop.pl")
            response = self.con.getresponse()
            self.assertEqual(response.status, HTTPStatus.REQUEST_TIMEOUT)
            assert_header(response, "Connection", "close")
            assert_body_binary(response, TIMEOUT_FILE_PATH)
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    def test_script_not_found_pl(self):
        try:
            self.con.request("GET", "/cgi-bin/non.pl")
            response = self.con.getresponse()
            self.assertEqual(response.status, HTTPStatus.NOT_FOUND)
            assert_header(response, "Connection", "keep-alive")
            assert_body_binary(response, NOT_FOUND_FILE_PATH)
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    def test_not_allowed_method_delete_pl(self):
        try:
            self.con.request("DELETE", "/cgi-bin/print_ok.pl")
            response = self.con.getresponse()
            self.assertEqual(response.status, HTTPStatus.METHOD_NOT_ALLOWED)
            assert_header(response, "Connection", "keep-alive")
            assert_body_binary(response, METHOD_NOT_ALLOWED_FILE_PATH)
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    def test_multiple_cgi_requests(self):
        try:
            # print_ok.plへのリクエスト
            self.con.request("GET", "/cgi-bin/print_ok.pl")
            response1 = self.con.getresponse()
            assert_status_line(response1, HTTPStatus.OK)
            assert_header(response1, "Connection", "keep-alive")
            assert_header(response1, "Content-Type", "text/plain")
            self.assertEqual(response1.read().decode(), "OK\n")

            # print_err.shへのリクエスト
            self.con.request("GET", "/cgi-bin/print_err.sh")
            response2 = self.con.getresponse()
            assert_status_line(response2, HTTPStatus.OK)
            assert_header(response2, "Connection", "keep-alive")
            # ファイルが実行されずに中身が返ってくればOK
            assert_body(response2, "root/cgi-bin/print_err.sh")

            # json.plへのリクエスト
            self.con.request("GET", "/cgi-bin/json.pl")
            response3 = self.con.getresponse()
            assert_status_line(response3, HTTPStatus.OK)
            assert_header(response3, "Connection", "keep-alive")
            assert_header(response3, "Content-Type", "application/json")
            expected_json = '{\n  "status": "success",\n  "message": "Hello, world!"\n}'
            self.assertEqual(response3.read().decode(), expected_json)
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    def test_multiple_http_requests(self):
        try:
            # local_redirect.plへのリクエスト
            self.con.request("GET", "/cgi-bin/local_redirect.pl")
            response = self.con.getresponse()
            # test_print_env_plと同じ内容が返ってくるはず
            assert_status_line(response, HTTPStatus.OK)
            assert_header(response, "Connection", "keep-alive")
            body = response.read().decode()
            self.assertIn("AUTH_TYPE:", body)
            self.assertIn("CONTENT_LENGTH:", body)
            self.assertIn("CONTENT_TYPE:", body)
            self.assertIn("GATEWAY_INTERFACE:", body)
            self.assertIn("PATH_INFO:", body)
            self.assertIn("PATH_TRANSLATED:", body)
            self.assertIn("QUERY_STRING:", body)
            self.assertIn("REMOTE_ADDR:", body)
            self.assertIn("REMOTE_HOST:", body)
            self.assertIn("REMOTE_IDENT:", body)
            self.assertIn("REMOTE_USER:", body)
            self.assertIn("REQUEST_METHOD:", body)
            self.assertIn("SCRIPT_NAME:", body)
            self.assertIn("SERVER_NAME:", body)
            self.assertIn("SERVER_PORT:", body)
            self.assertIn("SERVER_PROTOCOL:", body)
            self.assertIn("SERVER_SOFTWARE:", body)

            # GET /
            self.con.request("GET", "/")
            response = self.con.getresponse()
            assert_status_line(response, HTTPStatus.OK)
            assert_header(response, "Connection", "keep-alive")
            assert_body(response, "root/html/index.html")

            # client_redirect.plへのリクエスト
            self.con.request("GET", "/cgi-bin/client_redirect.pl")
            response = self.con.getresponse()
            assert_status_line(response, HTTPStatus.FOUND)
            assert_header(response, "Connection", "keep-alive")
            assert_header(response, "Location", "http://localhost:8080/")
            # bodyは空
            self.assertEqual(response.read().decode(), "")
        except HTTPException as e:
            self.fail(f"Request failed: {e}")
