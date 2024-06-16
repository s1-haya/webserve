    def test_not_found(self, port=8080) -> None:
        try:
            con = http.client.HTTPConnection("localhost", port)
            con.request("GET", "/nonexistent")
            response = con.getresponse()
            assert response.status == 404, f"Expected 404, got {response.status}"
            assert response.reason == "Not Found", f"Expected 'Not Found', got {response.reason}"
            print("test_not_found passed")
            con.close()
        except http.client.HTTPException as e:
            print(f"Request failed: {e}")
