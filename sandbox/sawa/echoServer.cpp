#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

const int PORT = 8080;
const int BACKLOG = 5;
const int BUFFER_SIZE = 1024;

int main() {
    // ソケットを作成
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    // アドレス設定
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // バインド
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == -1) {
        std::cerr << "Failed to bind\n";
        return 1;
    }

    // リスン
    if (listen(server_fd, BACKLOG) == -1) {
        std::cerr << "Failed to listen\n";
        return 1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        // 接続を受け入れ
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd == -1) {
            std::cerr << "Failed to accept connection\n";
            continue;
        }

        // クライアントからデータを受信
        char buffer[BUFFER_SIZE] = {0};
        ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (bytes_received == -1) {
            std::cerr << "Failed to receive data from client\n";
            close(client_fd);
            continue;
        }

        // クライアントにデータを送信
        ssize_t bytes_sent = send(client_fd, buffer, bytes_received, 0);
        if (bytes_sent == -1) {
            std::cerr << "Failed to send data to client\n";
        }

        // コネクションをクローズ
        close(client_fd);
    }

    // ソケットをクローズ
    close(server_fd);

    return 0;
}
