from http import HTTPStatus
from http.client import HTTPConnection
from typing import Optional


def assert_status_line(response: HTTPConnection, expected_code: HTTPStatus) -> None:
    assert (
        response.status == expected_code
    ), f"Expected status code {expected_code}, but got {response.status}"
    assert (
        response.reason == expected_code.phrase
    ), f"Expected status code reason {expected_code.phrase}, but got {response.reason}"
    assert response.version == 11, f"Expected version 1.1, but got {response.version}"


def assert_header(
    response: HTTPConnection,
    expected_header_name: Optional[str],
    expected_header_value: Optional[str],
) -> None:
    response_header_value = response.getheader(expected_header_name)
    # Header nameが存在するかどうか
    if response_header_value is None:
        raise ValueError(f"Header {expected_header_name} not found in the response")
    # Header valueは末尾に空白があるためstrip()使用
    response_header_value = response_header_value.strip()
    assert (
        response_header_value == expected_header_value
    ), f"Expected header value {expected_header_value}, but got {response_header_value}"


def assert_body(response: HTTPConnection, path: Optional[str]) -> None:
    response_body = response.read()
    expected_body = b""
    with open(path) as f:
        expected_body = f.read().encode("utf-8")
    assert (
        response_body == expected_body
    ), f"Expected response body {expected_body}, but got {response_body}"
    assert_header(response, "Content-Length", str(len(expected_body)))
