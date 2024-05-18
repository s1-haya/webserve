#include <iostream>
#include <unistd.h>
#include <cstring>
#include <poll.h>
#include <errno.h>
#include <vector>

// 非ブロッキング書き込み関数
void nonblocking_write(int fd, const char* data, size_t len) {
    size_t total_written = 0;
    while (total_written < len) {
        // pollfd構造体を初期化
        struct pollfd pfd;
        pfd.fd = fd;
        pfd.events = POLLOUT;
        
        // 書き込み可能になるのを待つ
        int result = poll(&pfd, 1, -1); // -1は無限に待つ
        if (result == -1) {
            perror("poll");
            break;
        }
        
        if (pfd.revents & POLLOUT) {
            // 書き込み可能になったら書き込む
            ssize_t written = write(fd, data + total_written, len - total_written);
            if (written > 0) {
                total_written += written;
            } else if (written == -1) {
                if (errno != EAGAIN && errno != EWOULDBLOCK) {
                    perror("write");
                    break;
                }
            }
        }
    }
}

int main() {
    const char* message = "Hello, non-blocking world!\n";
    const int num_pipes = 3;
    std::vector<int> pipes(num_pipes * 2); // 各パイプに2つのファイルディスクリプタ（読み取り用と書き込み用）

    // パイプを作成
    for (int i = 0; i < num_pipes; ++i) {
        if (pipe(&pipes[i * 2]) == -1) {
            perror("pipe");
            return 1;
        }
    }

    // 複数の子プロセスを作成して非ブロッキング書き込みをテスト
    for (int i = 0; i < num_pipes; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return 1;
        } else if (pid == 0) {
            // 子プロセス
            close(pipes[i * 2 + 1]); // 子プロセスは書き込み用ファイルディスクリプタを閉じる

            // 子プロセスがデータを読み取る
            char buffer[256];
            ssize_t bytes_read = read(pipes[i * 2], buffer, sizeof(buffer) - 1);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                std::cout << "Child " << i << " received: " << buffer << std::endl;
            }

            close(pipes[i * 2]);
            return 0;
        }
    }

    // 親プロセス
    for (int i = 0; i < num_pipes; ++i) {
        close(pipes[i * 2]); // 親プロセスは読み取り用ファイルディスクリプタを閉じる
        nonblocking_write(pipes[i * 2 + 1], message, strlen(message));
        close(pipes[i * 2 + 1]);
    }

    // 親プロセスが子プロセスの終了を待つ
    for (int i = 0; i < num_pipes; ++i) {
        wait(nullptr);
    }

    return 0;
}
