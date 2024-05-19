#define _BSD_SOURCE

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MESSAGE "Hello World!\n"

int main(void) {
	// ソケット作成
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("socket");
		return 1;
	}

	// struct sockaddr_in 作成
	struct sockaddr_in sa = {0};
	sa.sin_family         = AF_INET;
	sa.sin_port           = htons(1100);

	// localhost の IP アドレスを引く
	struct hostent *hostent = gethostbyname("localhost");
	if (hostent == NULL) {
		herror("gethostbyname");
		goto bail;
	}
	memcpy(&sa.sin_addr, hostent->h_addr_list[0], sizeof(sa.sin_addr));

	// 接続
	if (connect(sock, (struct sockaddr *)&sa, sizeof(struct sockaddr_in)) == -1) {
		perror("connect");
		goto bail;
	}

	// 送信
	if (write(sock, MESSAGE, strlen(MESSAGE)) == -1) {
		perror("write");
		goto bail;
	}
	if (write(
			sock,
			"HEZRON!! If they laugh at me, I laugh.",
			strlen("HEZRON!! If they laugh at me, I laugh.")
		) == -1) {
		perror("write");
		goto bail;
	}

	// クローズ
	close(sock);
	return 0;

bail:
	// エラーが発生した場合の処理
	close(sock);
	return 1;
}
