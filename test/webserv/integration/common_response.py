from common_functions import read_file, read_file_binary

KEEP_ALIVE = "keep-alive"
CLOSE = "close"
TEXT_HTML = "text/html"


def create_response_header(
    status_code, reason_phrase, connection, content_length, content_type="text/plain"
):
    return f"HTTP/1.1 {status_code} {reason_phrase}\r\nConnection: {connection}\r\nContent-Length: {content_length}\r\nContent-Type: {content_type}\r\nServer: webserv/1.1\r\n\r\n"


root_index_file, root_index_file_length = read_file("root/html/index.html")
sub_index_file, sub_index_file_length = read_file("root/html/sub/index.html")

error_files = [
    ("400_bad_request.txt", "bad_request_file_400", "bad_request_file_400_length"),
    ("404_not_found.txt", "not_found_file_404", "not_found_file_404_length"),
    (
        "405_method_not_allowed.txt",
        "not_allowed_file_405",
        "not_allowed_file_405_length",
    ),
    ("408_timeout.txt", "timeout_file_408", "timeout_file_408_length"),
    (
        "501_not_implemented.txt",
        "not_implemented_file_501",
        "not_implemented_file_501_length",
    ),
]

for filename, data_var, length_var in error_files:
    data, length = read_file_binary(
        f"test/webserv/expected_response/default_body_message/{filename}"
    )
    globals()[data_var] = data
    globals()[length_var] = length


response_header_get_root_200_close = create_response_header(
    200, "OK", CLOSE, root_index_file_length, TEXT_HTML
)
response_header_get_root_200_keep = create_response_header(
    200, "OK", KEEP_ALIVE, root_index_file_length, TEXT_HTML
)
response_header_get_sub_200_close = create_response_header(
    200, "OK", CLOSE, sub_index_file_length, TEXT_HTML
)
response_header_400 = create_response_header(
    400, "Bad Request", CLOSE, bad_request_file_400_length, TEXT_HTML
)
response_header_404 = create_response_header(
    404, "Not Found", CLOSE, not_found_file_404_length, TEXT_HTML
)
response_header_405 = create_response_header(
    405, "Method Not Allowed", CLOSE, not_allowed_file_405_length, TEXT_HTML
)
response_header_408 = create_response_header(
    408, "Request Timeout", CLOSE, timeout_file_408_length, TEXT_HTML
)
response_header_501 = create_response_header(
    501, "Not Implemented", CLOSE, not_implemented_file_501_length, TEXT_HTML
)

bad_request_response = response_header_400 + bad_request_file_400.decode("utf-8")
not_found_response = response_header_404 + not_found_file_404.decode("utf-8")
not_allowed_response = response_header_405 + not_allowed_file_405.decode("utf-8")
timeout_response = response_header_408 + timeout_file_408.decode("utf-8")
not_implemented_response = response_header_501 + not_implemented_file_501.decode(
    "utf-8"
)
