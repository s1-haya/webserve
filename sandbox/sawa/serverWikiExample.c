#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void)
{
    // サーバーソケット作成
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("socket");
        return 1;
    }

    // struct sockaddr_in 作成
    struct sockaddr_in sa = {0};
    sa.sin_family = AF_INET;
    sa.sin_port = htons(1100);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    // バインド
    if (bind(sock, (struct sockaddr*) &sa, sizeof(struct sockaddr_in)) == -1)
    {
        perror("bind");
        goto bail;
    }

    // リッスン
    if (listen(sock, 128) == -1)
    {
        perror("listen");
        goto bail;
    }

    while (1)
    {
        // クライアントの接続を待つ
        int fd = accept(sock, NULL, NULL);
        if (fd == -1)
        {
            perror("accept");
            goto bail;
        }

        // 受信
        char buffer[4096];
        int recv_size = read(fd, buffer, sizeof(buffer) - 1);
        if (recv_size == -1)
        {
            perror("read");
            close(fd);
            goto bail;
        }

        // 受信内容を表示
        buffer[recv_size] = '\0';
        printf("message: %s\n", buffer);

        // ソケットのクローズ
        if (close(fd) == -1)
        {
            perror("close");
            goto bail;
        }
    }

bail:
    // エラーが発生した場合の処理
    close(sock);
    return 1;
}
