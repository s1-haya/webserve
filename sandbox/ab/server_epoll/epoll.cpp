#include "epoll.hpp"
#include <stdio.h>     // perror
#include <sys/epoll.h> // epoll
#include <unistd.h>    // close

#define BUFFER_SIZE 1024

/*
int epoll_create(int size);
int epoll_create1(int flags);

新規作成したepoll instanceに対応するfdを返す
以降このfdを用いてepoll instance操作する
- size :　監視対象fd数だが、カーネル内の初期化処理に対するヒントに過ぎず、実際は
		  この値を超えた数のfdも処理可能
*/
int CreateEpollInstance(int server_fd) {
	int epoll_fd = epoll_create1(EPOLL_CLOEXEC);
	if (epoll_fd == -1) {
		perror("epoll_create failed");
		close(server_fd);
	}
	return epoll_fd;
}

/*
int epoll_ctl(int epoll_fd, int op, int fd, struct epoll_event *ev);

interest listを変更する
- epoll_fd : epoll instance
- op : 変更内容を指定
- fd : interest list内のfd(=監視対象fd)
*/
bool AddSocketToEpollInstance(int epoll_fd, int server_fd, struct epoll_event *ev) {
	ev->events  = EPOLLIN;
	ev->data.fd = server_fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, ev) == -1) {
		perror("epoll_ctl failed");
		close(server_fd);
		return false;
	}
	return true;
}

/*
int epoll_wait(int epoll_fd, struct epoll_event *evlist, int maxevents, int
timeout);

I/O可能なfdに関数情報を取得する。一度に複数のI/O可能なfd情報を取得できる
evlistに代入した要素数を返す
- epoll_fd : epoll instance
- evlist : この構造体の配列にI/O可能fd情報が返る
- maxevents : 要素数
- timeout : ブロック動作を操作(-1,0,正)
*/
int WaitEpollEvent(int epoll_fd, struct epoll_event *evlist, int maxevents) {
	int ready = epoll_wait(epoll_fd, evlist, maxevents, -1);
	if (ready == -1) {
		// if (errno == EINTR) continue;
		perror("epoll_wait failed");
	}
	return ready;
}

/*
ssize_t send(int sockfd, const void *buffer, size_t length, int flags);

接続済みソケットに対しI/Oを実行する
- return,sockfd,buffer,length : read(),write()と同じ
- flags : 操作を操作するbit mask
*/
void ReadAndSendBackClientData(int client_fd, int epoll_fd) {
	char buffer[BUFFER_SIZE];

	ssize_t read_ret = read(client_fd, buffer, BUFFER_SIZE);
	if (read_ret <= 0) {
		close(client_fd);
		epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
	} else {
		send(client_fd, buffer, read_ret, 0);
	}
}
