from http import HTTPStatus
from http.client import HTTPConnection, HTTPException

import pytest
from http_module.assert_http_response import (assert_body, assert_header,
                                              assert_status_line)


def test_ok() -> None:
    try:
        con = HTTPConnection("localhost", 8080)
        con.request("GET", "/")
        response = con.getresponse()
        assert_status_line(response, HTTPStatus.OK)
        assert_header(response, "Connection", "keep-alive")
        assert_body(response, "root/html/index.html")
        con.close()
    except HTTPException as e:
        print(f"Request failed: {e}")


def test_ok_in_sub_directory() -> None:
    try:
        con = HTTPConnection("localhost", 8080)
        con.request("GET", "/sub/")
        response = con.getresponse()
        assert_status_line(response, HTTPStatus.OK)
        assert_header(response, "Connection", "keep-alive")
        assert_body(response, "root/html/sub/index.html")
        con.close()
    except HTTPException as e:
        print(f"Request failed: {e}")


@pytest.mark.skip(reason="cannot connect to 8000 from out side of container")
def test_not_found() -> None:
    try:
        # error_pageのテスト
        con = HTTPConnection("localhost", 8000)
        con.request("GET", "/sub/sss")
        response = con.getresponse()
        assert_status_line(response, HTTPStatus.NOT_FOUND)
        assert_header(response, "Connection", "keep-alive")
        assert_body(response, "root/html/error_pages/404.html")
        con.close()
    except HTTPException as e:
        print(f"Request failed: {e}")
