#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

#define DATA "Half a league, half a league . . ."

/*
 * このプログラムはソケットを作成し、コマンド行で指定
 * されるソケットを使用して接続を開始します。この接続で
 * データがいくらか送信されたあとソケットが閉じられ、
 * 接続が終了します。
 * コマンド行の書式: streamwrite hostname portnumber
 * 使用法: pgm host port
 */
int main(int argc, char *argv[]) {
	int sock, errnum h_addr_index;
	struct sockaddr_in6 server;
	struct hostent     *hp;
	char                buf[1024];

	/* ソケットを作成する。*/
	sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("opening stream socket");
		exit(1);
	}
	/* コマンド行で指定される名前を使用してソケットを接続する。*/
	bzero(&sin6, sizeof(sin6));
	server.sin6_family = AF_INET6;
	hp                 = getipnodebyname(AF_INET6, argv[1], AI_DEFAULT, &errnum);
	/*
	 * getinodebyname が、指定されたホストのネットワーク
	 * アドレスを含む構造体を返す。
	 */
	if (hp == (struct hostent *)0) {
		fprintf(stderr, "%s: unknown host¥n", argv[1]);
		exit(2);
	}

	h_addr_index = 0;
	while (hp->h_addr_list[h_addr_index] != NULL) {
		bcopy(hp->h_addr_list[h_addr_index], &server.sin6_addr, hp->h_length);
		server.sin6_port = htons(atoi(argv[2]));
		if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
			if (hp->h_addr_list[++h_addr_index] != NULL) {
				/* 次のアドレスを試みる */
				continue;
			}
			perror("connecting stream socket");
			freehostent(hp);
			exit(1);
		}
		break;
	}
	freehostent(hp);
	if (write(sock, DATA, sizeof DATA) == -1)
		perror("writing on stream socket");
	close(sock);
	freehostent(hp);
	exit(0);
}
