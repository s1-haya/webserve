#ifndef EPOLL_HPP
#define EPOLL_HPP

#include <sys/socket.h> // accept,send,socklen_t

int  CreateEpollInstance(int server_fd);
bool AddSocketToEpollInstance(int epoll_fd, int server_fd, struct epoll_event *ev);
int  WaitEpollEvent(int epoll_fd, struct epoll_event *evlist, int maxevents);
bool AcceptNewClient(
	int                 server_fd,
	int                 epoll_fd,
	struct sockaddr    *sock_addr,
	socklen_t          *addrlen,
	struct epoll_event *ev
);
void ReadAndSendBackClientData(int client_fd, int epoll_fd);

#endif /* EPOLL_HPP */
