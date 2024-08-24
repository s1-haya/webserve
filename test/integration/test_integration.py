import client_module.client as client

def test_webserv() -> None:
    try:
        client_instance = client.Client(8080)
        response = client_instance.SendRequestAndReceiveResponse("GET / HTTP/1.1\nHost: localhost\nConnection: close\r\n\r\n")
        print(response)
        
    except Exception as e:
        print(e)
        assert False


if __name__ == "__main__":
    test_webserv()
