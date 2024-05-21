#include "bind.hpp"
#include <stdio.h>  // perror
#include <unistd.h> // close

/*
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

ソケットをアドレスへバインドする
- sockfd : 先に実行したsocket()により得たfd
- addr : bindするアドレスを表す構造体へのポインタ
- addrlen : addrのサイズ

struct sockaddr : 汎用ソケットアドレス構造体
全種類のドメインに対応し、各ドメインに依存した専用のアドレス構造体を引数に使用できるよう、
一意な構造体へキャストする目的
struct sockaddr {
	sa_family_t	sa_family;   // アドレスファミリ(AF_*マクロ)
	char		sa_data[14]; // ソケットアドレス(サイズはドメインに依存)
};
*/
bool BindSocket(int server_fd, const struct sockaddr *addr, socklen_t addrlen) {
	if (bind(server_fd, addr, addrlen) == -1) {
		perror("bind failed");
		close(server_fd);
		return false;
	}
	return true;
}
