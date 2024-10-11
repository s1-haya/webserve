from http import HTTPStatus
from http.client import HTTPResponse


def assert_status_line(response: HTTPResponse, expected_code: HTTPStatus) -> None:
    assert (
        response.status == expected_code
    ), f"Expected status code {expected_code}, but got {response.status}"
    assert (
        response.reason == expected_code.phrase
    ), f"Expected status code reason\n\n {expected_code.phrase}, but got\n\n {response.reason}"
    assert response.version == 11, f"Expected version 1.1, but got {response.version}"


def assert_header(
    response: HTTPResponse,
    expected_header_name: str,
    expected_header_value: str,
) -> None:
    response_header_value = response.getheader(expected_header_name)
    # Header nameが存在するかどうか
    if response_header_value is None:
        raise ValueError(f"Header {expected_header_name} not found in the response")
    # Header valueは末尾に空白があるためstrip()使用
    response_header_value = response_header_value.rstrip()
    assert (
        response_header_value == expected_header_value
    ), f"Expected header value\n\n {expected_header_value}, but got\n\n {response_header_value}"


def assert_body(response: HTTPResponse, path: str) -> None:
    response_body = response.read()
    expected_body = b""
    with open(path, "rb") as f:
        expected_body = f.read()
    assert (
        response_body == expected_body
    ), f"Expected response body\n\n {expected_body}\n, but got\n\n {response_body}"
    assert_header(response, "Content-Length", str(len(expected_body)))
