import subprocess
import time
import requests

test_webserv_dir = 'test/webserv/'
container_name = 'webserv-container'

def run_command(command, working_dir):
    try:
        result = subprocess.run(command, cwd=working_dir, check=True, capture_output=True, text=True)
        return result
    except subprocess.CalledProcessError as e:
        print(f"An error occurred: {e.stderr}")
        return None

#コンテナの起動を確認する関数
def wait_for_container(container_name, timeout=60, interval=10):
    start_time = time.time()
    while time.time() - start_time < timeout:
        result = run_command(['make', 'ps'], test_webserv_dir)
        if container_name in result.stdout:
            print(f"Container {container_name} is up and running!")
            return True
        else:
            print(f"Container {container_name} not ready, waiting...")
        time.sleep(interval)
    print(f"Container {container_name} did not start within the timeout period.")
    return False

def test_wevserv():
    # イメージをビルドし、コンテナを起動
    run_command(['make', 'build'], test_webserv_dir)
    run_command(['make', 'run'], test_webserv_dir)
    if wait_for_container(container_name):
        try:
            # 起動後sleepしないとcurl: (56) Recv failure: Connection reset by peerになるため
            time.sleep(5)
            server_url = "http://localhost:8080"
            cp = subprocess.run(['curl', server_url], capture_output=True, text=True)
            # レスポンスのコンテントが出力されたらOK
            print(cp.stdout)
        except subprocess.CalledProcessError as e:
            print(f"Request failed: {e}")

    # コンテナを停止し削除
    result = run_command(['make', 'fclean'], test_webserv_dir)
    if result is not None:
        print(f"Container {container_name} is down and remove!")
