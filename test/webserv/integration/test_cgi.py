import unittest
from http_module.assert_http_response import assert_body, assert_header, assert_status_line
from http.client import HTTPConnection, HTTPException
from http import HTTPStatus

class TestCGI(unittest.TestCase):
    def setUp(self):
        # 各テストの前に実行される(unittestの機能)
        self.con = HTTPConnection("localhost", 8080)

    def tearDown(self):
        # 各テストの後に実行される(unittestの機能)
        self.con.close()

    def test_print_ok_pl(self):
        try:
            self.con.request("GET", "/cgi-bin/print_ok.pl")
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
        except HTTPException as e:
            self.fail(f"Request failed: {e}")
    
    def test_print_ok_py(self):
        try:
            self.con.request("GET", "/cgi-bin/print_ok.py")
            response = self.con.getresponse()
            assert_status_line(response, HTTPStatus.OK)
            assert_header(response, "Connection", "keep-alive")
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
            self.assertIn(b"No POST data received.", response.read())
        except HTTPException as e:
            self.fail(f"Request failed: {e}")

    # todo: PathInfo(マージ後追加)
    # todo: error (マージ後追加)
            
