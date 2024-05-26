#include "accept.hpp"
#include "bind.hpp"
#include "epoll.hpp"
#include "listen.hpp"
#include "server.hpp"
#include "socket.hpp"
#include <cstdlib>
#include <sys/epoll.h> // epoll
#include <unistd.h>    // close

#define MAX_EVENTS 1024

/*
struct sockaddr_in : IPv4ソケットアドレス

struct in_addr { 			   // IPv4アドレスは4byte
	in_addr_t s_addr; 		   // 符号なし32bit整数
};

struct sockaddr_in {           // IPv4ソケットアドレス
	sa_family_t    sin_family; // アドレスファミリ(AF_INET)
	in_port_t      sin_port;   // ポート番号(符号なし16bit整数)
	struct in_addr sin_addr;   // IPv4アドレス
	unsigned char  __pad[X];   // sockaddr構造体に合わせるパディング(16byte)
};
*/
int main() {
	struct sockaddr_in sock_addr;
	std::size_t        addrlen = sizeof(sock_addr);

	// socket
	const int server_fd = CreateSocket();
	if (server_fd == -1) {
		return EXIT_FAILURE;
	}
	SetSocketAddr(sock_addr);

	// bind
	if (!BindSocket(server_fd, (const struct sockaddr *)&sock_addr, addrlen)) {
		return EXIT_FAILURE;
	}

	// listen
	if (!Listen(server_fd)) {
		return EXIT_FAILURE;
	}

	// epoll
	const int epoll_fd = CreateEpollInstance(server_fd);
	if (epoll_fd == -1) {
		return EXIT_FAILURE;
	}
	/*
	struct epoll_event {
		uint32_t		events; // epollイベント(ビットマスク)
		epoll_data_t	data;   // ユーザデータ
	};
	*/
	struct epoll_event ev;
	if (!AddSocketToEpollInstance(epoll_fd, server_fd, &ev)) {
		return EXIT_FAILURE;
	}

	struct epoll_event evlist[MAX_EVENTS];
	while (true) {
		const ssize_t ready = WaitEpollEvent(epoll_fd, evlist, MAX_EVENTS);
		if (ready == -1) {
			return EXIT_FAILURE;
		}
		for (std::size_t i = 0; i < (std::size_t)ready; ++i) {
			if (evlist[i].data.fd == server_fd) {
				// accept
				const int new_socket = AcceptNewClient(
					server_fd, (struct sockaddr *)&sock_addr, (socklen_t *)&addrlen
				);
				if (new_socket == -1) {
					return EXIT_FAILURE;
				}
				if (!AddSocketToEpollInstance(epoll_fd, new_socket, &ev)) {
					return EXIT_FAILURE;
				}
			} else {
				// read,send
				const int client_fd = evlist[i].data.fd;
				ReadAndSendBackClientData(client_fd, epoll_fd);
			}
		}
	}
	close(server_fd);

	return EXIT_SUCCESS;
}
