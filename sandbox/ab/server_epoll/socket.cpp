#include "socket.hpp"
#include <arpa/inet.h> // htons
#include <stdio.h>     // perror

#define PORT 8080

/*
int socket(int domain, int type, int protocol);

通信のエンドポイントを作成しfdを返す
- domain : ソケットの通信を行う通信ドメイン。使用するプロトコルファミリを選択
- type : ソケット通信の種類
- protocol : ソケットで使用する特定プロトコルを指定

int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t
optlen);

- sockfd : ソケットに対応するfd
- level : プロトコルレベル。ソケットAPIレベルはSOL_SOCKET
- optnaem : 操作対象のソケットオプション
	SO_REUSEADDR : 前回実行したサーバがクライアントと通信しアクティブクローズを
				   実行したなど、動作中のポートへソケットをバインドする際に発生する
				   エラー(EADDRINUSE)を防ぐ
- optval : ソケットオプションの値を持つ変数のアドレス
- optlen : optvalが指すバッファのサイズ
*/
int CreateSocket() {
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		perror("socket failed");
	}
	// set socket option to reuse address
	int optval = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) ==
		-1) {
		perror("setsockopt failed");
	}
	return server_fd;
}

/*
uint16_t htons(uint16_t host_uint16);

整数をホストバイトオーダ,ネットワークバイトオーダへ変換する関数(通常はマクロ)
*/
void SetSocketAddr(struct sockaddr_in &sock_addr) {
	sock_addr.sin_family      = AF_INET;
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	sock_addr.sin_port        = htons(PORT);
}
