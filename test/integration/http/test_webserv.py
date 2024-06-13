import subprocess
import time
import requests

def test_wevserv():
    try:
        # 起動後sleepしないとcurl: (56) Recv failure: Connection reset by peerになるため
        time.sleep(5)
        server_url = "http://localhost:8080"
        cp = subprocess.run(['curl', server_url], capture_output=True, text=True)
        # レスポンスのコンテントが出力されたらOK
        print(cp.stdout)
    except subprocess.CalledProcessError as e:
        print(f"Request failed: {e}")

if __name__ == "__main__":
    test_webserv()
