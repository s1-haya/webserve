import http.client

def send_chunked_request(file_path):
    conn = http.client.HTTPConnection("localhost", 80)
    conn.putrequest("PUT", "/upload/a.txt")
    conn.putheader("Transfer-Encoding", "chunked")
    conn.endheaders()

    with open(file_path, 'rb') as f:
        while True:
            chunk = f.read(1)
            print(chunk)
            if not chunk:
                break
            # チャンクサイズを16進数で送信
            conn.send(f"{len(chunk):X}\r\n".encode())
            conn.send(chunk + b"\r\n")
    # チャンクの終了を示す0を送信
    conn.send(b"0\r\n\r\n")

    response = conn.getresponse()
    print(response.status, response.reason)
    print(response.read().decode())

send_chunked_request('request.txt')

