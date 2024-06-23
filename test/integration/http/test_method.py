from assert_http_response import assert_status_line
from http.client import HTTPConnection, HTTPException
from http import HTTPStatus

class TestStatusCode:
    def test_ok(self):
        try:
            con = HTTPConnection("localhost", 8080)
            con.request("GET", "/")
            response = con.getresponse()
            assert_status_line(HTTPStatus.OK, response)
            con.close()
        except http.client.HTTPException as e:
            print(f"Request failed: {e}")
