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
    if file_path is None:
        return

    try:
        if os.path.exists(file_path):
            os.remove(file_path)
            print(f"Deleted file: {file_path}")
    except Exception as e:
        print(f"Error deleting file: {file_path}, {e}")
        assert False


# テスト前に削除しておきたいupload_file_pathが特になければNoneを指定する
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
    # testで作成したいファイルがあるならtest前に念のため削除
    delete_file(upload_file_path)

    send_request_and_assert_response(request_file, expected_response)

    # testで作成したファイルがあれば次のtestのために削除
    delete_file(upload_file_path)
