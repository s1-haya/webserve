#include <stdio.h>      // perror
#include <sys/socket.h> // listen
#include <unistd.h>     // close

/*
int listen(int sockfd, int backlog);

sockfdのストリームソケットをパッシブソケットとマークする
以降、このソケットはアクティブソケットからの接続要求を受け入れる
- backlog : 保留中の接続要求の個数の上限(実際はキューとか色々)
			LinuxではSOMAXCONN=128
*/
bool Listen(int server_fd) {
	if (listen(server_fd, 3) == -1) {
		perror("listen failed");
		close(server_fd);
		return false;
	}
	return true;
}
