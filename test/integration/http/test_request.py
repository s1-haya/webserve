import requests
import unittest

class TestStatusCode(unittest.TestCase):
    def test_status_code_ok(self):
        # webservにHTTP GETリクエストを送信
        response = requests.get("http://localhost:8080")
        # ステータスコードが200であることを確認
        self.assertEqual(response.status_code, 200)
        # webservのレスポンス
        print(response)
        # webservのレスポンスのデータ
        print(response.text)
        # print(response.headers())

if __name__ == "__main__":
    unittest.main()
