from assert_http_response import assert_status_line, assert_header, assert_body
from http.client import HTTPConnection, HTTPException
from http import HTTPStatus

class TestStatusCode:
    def test_ok(self):
        try:
            con = HTTPConnection("localhost", 8080)
            con.request("GET", "/")
            response = con.getresponse()
            assert_status_line(HTTPStatus.OK, response)
            assert_header("Connection", "close", response)
            assert_body("./html/index.html", response)
            con.close()
        except HTTPException as e:
            print(f"Request faild: {e}" )

    def test_ok_in_sub_directory(self):
        try:
            con = HTTPConnection("localhost", 8080)
            con.request("GET", "/sub")
            response = con.getresponse()
            assert_status_line(HTTPStatus.OK, response)
            assert_header("Connection", "close", response)
            assert_body("./html/sub/index.html", response)
            con.close()
        except HTTPException as e:
            print(f"Request faild: {e}" )

    def test_not_found(self):
        try:
            con = HTTPConnection("localhost", 8080)
            con.request("GET", "/no-exist-path")
            response = con.getresponse()
            assert_status_line(HTTPStatus.NOT_FOUND, response)
            assert_header("Connection", "close", response)
            assert_body("./html/404.html", response)
            con.close()
        except HTTPException as e:
            print(f"Request faild: {e}" )
