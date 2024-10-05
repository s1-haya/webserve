import os

import pytest
from common_functions import read_file, send_request_and_assert_response
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


def assert_uploaded_file_content(upload_file_path, expected_upload_file_content):
    if upload_file_path is None:
        return

    result_body_message, result_content_length = read_file(upload_file_path)

    assert result_body_message == expected_upload_file_content
    assert result_content_length == len(expected_upload_file_content)


# upload_file_path: テスト前に削除しておきたいupload_file_pathが特になければNoneを指定
# expected_upload_file_content: ファイル作成された場合に期待するファイルの中身
@pytest.mark.parametrize(
    "request_file, expected_response, upload_file_path, expected_upload_file_content",
    [
        (
            REQUEST_POST_2XX_DIR + "201_01_upload_file.txt",
            created_response,
            UPLOAD_FILE_PATH,
            "abcde",
        ),
        (
            REQUEST_POST_2XX_DIR + "201_02_chunked.txt",
            created_response,
            UPLOAD_DIR + "chunked_request_file",
            "SAWAMURA HAYATO",
        ),
    ],
    ids=[
        "201_01_upload_file",
        "201_02_chunked",
    ],
)
def test_post_upload_responses(
    request_file, expected_response, upload_file_path, expected_upload_file_content
):
    # testで作成したいファイルがあるならtest前に念のため削除
    delete_file(upload_file_path)

    send_request_and_assert_response(request_file, expected_response)
    assert_uploaded_file_content(upload_file_path, expected_upload_file_content)

    # testで作成したファイルがあれば次のtestのために削除
    delete_file(upload_file_path)
