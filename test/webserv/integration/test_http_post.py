import os

import pytest
from common_functions import send_request_and_assert_response
from common_response import *

REQUEST_DIR = "test/common/request/"
REQUEST_POST_2XX_DIR = REQUEST_DIR + "post/2xx/"
REQUEST_POST_4XX_DIR = REQUEST_DIR + "post/4xx/"

UPLOAD_DIR = "root/upload/"
UPLOAD_FILE_PATH = UPLOAD_DIR + "test_upload_file"


def delete_file(file_path):
    try:
        if os.path.exists(file_path):
            os.remove(file_path)
            print(f"Deleted file: {file_path}")
    except Exception as e:
        print(f"Error deleting file: {file_path}, {e}")
        assert False


@pytest.mark.parametrize(
    "request_file, expected_response, upload_file_path",
    [
        (
            REQUEST_POST_2XX_DIR + "201_01_upload_file.txt",
            created_response,
            UPLOAD_FILE_PATH,
        ),
    ],
    ids=[
        "201_01_upload_file",
    ],
)
def test_post_responses(request_file, expected_response, upload_file_path):
    delete_file(upload_file_path)
    send_request_and_assert_response(request_file, expected_response)
    delete_file(upload_file_path)
