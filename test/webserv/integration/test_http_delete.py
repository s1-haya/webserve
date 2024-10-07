import os
import time

import pytest
from common_functions import read_file, send_request_and_assert_response
from common_response import (bad_request_response, no_content_response_close,
                                no_content_response_keep)

REQUEST_DIR = "test/common/request/"
REQUEST_DELETE_2XX_DIR = REQUEST_DIR + "delete/2xx/"
REQUEST_DELETE_4XX_DIR = REQUEST_DIR + "delete/4xx/"

UPLOAD_DIR = "root/upload/"
DELETE_FILE_PATH = UPLOAD_DIR + "delete_file"
NOT_EXISTING_FILE_PATH = UPLOAD_DIR + "non_existing_file"

def create_file(file_path, content=""):
    if file_path is None:
        return

    try:
        os.makedirs(os.path.dirname(file_path), exist_ok=True)
        with open(file_path, 'w') as f:
            f.write(content)
        print(f"Created file: {file_path}")
    except Exception as e:
        print(f"Error creating file: {file_path}, {e}")
        raise AssertionError


@pytest.fixture
def setup_file_context():
    # コンテキストマネージャとして使用可能なフィクスチャ
    from contextlib import contextmanager

    @contextmanager
    def _setup(file_path, content=""):
        create_file(file_path, content)
        try:
            yield
        finally:
            if os.path.exists(file_path):
                os.remove(file_path)
                print(f"Deleted file: {file_path}")

    return _setup


@pytest.mark.parametrize(
    "request_file, expected_response, file_path, file_content",
    [
        (
            REQUEST_DELETE_2XX_DIR + "204_01_delete_existing_file.txt",
            no_content_response_close,
            DELETE_FILE_PATH,
            "Sample content for deletion",
        ),
    ],
    ids=[
        "204_01_delete_existing_file",
    ],
)
def test_delete_responses(
    request_file,
    expected_response,
    file_path,
    file_content,
    setup_file_context,
):
    # setup_file_contextフィクスチャを使用してファイル作成を実行
    if file_path:
        with setup_file_context(file_path, file_content):
            send_request_and_assert_response(request_file, expected_response)



# @pytest.mark.parametrize(
#     "request_file, expected_response, upload_file_path",
#     [
#         (
#             REQUEST_POST_4XX_DIR + "400_01_duplicate_content_length.txt",
#             bad_request_response,
#             UPLOAD_DIR + "duplicate_content_length",
#         ),
        # (
        #     REQUEST_DELETE_4XX_DIR + "400_01_delete_non_existing_file.txt",
        #     bad_request_response,
        #     NOT_EXISTING_FILE_PATH,
        #     "",
        # ),
#         (
#             REQUEST_POST_4XX_DIR + "400_02_transfer_encoding_and_content_length.txt",
#             bad_request_response,
#             UPLOAD_FILE_PATH,
#         ),
#         (
#             REQUEST_POST_4XX_DIR + "408_01_shortened_body_message.txt",
#             timeout_response,
#             UPLOAD_DIR + "shortened_body_message",
#         ),
#         (
#             REQUEST_POST_4XX_DIR + "408_02_no_body_message.txt",
#             timeout_response,
#             UPLOAD_DIR + "no_body_message",
#         ),
#     ],
#     ids=[
#         "400_01_duplicate_content_length",
#         "400_02_transfer_encoding_and_content_length",
#         "408_01_shortened_body_message",
#         "408_02_no_body_message",
#     ],
# )
# def test_post_4xx_responses(
#     request_file,
#     expected_response,
#     upload_file_path,
#     cleanup_file_context,
# ):
#     if upload_file_path:
#         with cleanup_file_context(upload_file_path):
#             send_request_and_assert_response(request_file, expected_response)
