#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

const int PORT = 8080;
const char* SERVER_IP = "127.0.0.1"; // ローカルホスト

int main() {
    // ソケットを作成
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    // サーバーに接続するためのアドレス設定
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    // サーバーに接続
    if (connect(client_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Failed to connect to server\n";
        return 1;
    }

    // リクエストの作成
    const char* request = "GET / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";

    // サーバーにリクエストを送信
    ssize_t bytes_sent = send(client_fd, request, strlen(request), 0);
    if (bytes_sent == -1) {
        std::cerr << "Failed to send request to server\n";
        close(client_fd);
        return 1;
    }

    // サーバーからのレスポンスを受信
    char buffer[1024];
    ssize_t bytes_received = recv(client_fd, buffer, 1024, 0);
    if (bytes_received == -1) {
        std::cerr << "Failed to receive response from server\n";
        close(client_fd);
        return 1;
    }

    // 受信したデータを標準出力に出力
    std::cout << "Response from server:\n" << std::string(buffer, bytes_received) << std::endl;

    // ソケットをクローズ
    close(client_fd);

    return 0;
}
