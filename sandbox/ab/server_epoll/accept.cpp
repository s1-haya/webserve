#include "accept.hpp"
#include <stdio.h>     // perror
#include <sys/epoll.h> // epoll

/*
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

sockfdのリスニングソケットに対しなされた接続要求を取り出す
- return : 新規ソケット(connect()確立済み)。NULL可
- addrlen : addrが指すbufferへ実際にコピーしたバイト数が代入される。NULL可
*/
int AcceptNewClient(int server_fd, struct sockaddr *sock_addr, socklen_t *addrlen) {
	int new_socket = accept(server_fd, sock_addr, addrlen);
	if (new_socket == -1) {
		perror("accept failed");
		return new_socket;
	}
	return new_socket;
}
