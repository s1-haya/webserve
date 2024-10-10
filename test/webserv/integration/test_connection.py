from http.client import HTTPConnection, HTTPException

from common_functions import SERVER_PORT


# ------------------------------------------------------------------------------
# serverがその後も正常に動いていればOK
def test_05_timeout_no_request_disconnect() -> None:
    try:
        con = HTTPConnection("localhost", SERVER_PORT)
        con.connect()

        # requestを何も送らない

        # 何も送らずに接続を切断
        con.close()

    except HTTPException as e:
        print(f"Request failed: {e}")
        raise AssertionError
    finally:
        if con:
            con.close()


# serverがその後も正常に動いていればOK
def test_08_shortened_body_message_disconnect() -> None:
    try:
        con = HTTPConnection("localhost", SERVER_PORT)
        con.connect()

        # content-length分以下のbody
        con.request(
            "GET",
            "/",
            "abc",
            {
                "Connection": "keep-alive",
                "Content-Length": "5",
            },
        )

        # content-length分を送る前に接続を切る
        con.close()

    except HTTPException as e:
        print(f"Request failed: {e}")
        raise AssertionError
    finally:
        if con:
            con.close()
