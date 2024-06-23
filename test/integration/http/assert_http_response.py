from http import HTTPStatus
from http.client import HTTPConnection

def assert_status_line(code: HTTPStatus, output: HTTPConnection) -> None:
	assert code == output.status
	assert code.phrase == output.reason
	assert 11 == output.version

