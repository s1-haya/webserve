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
