#ifndef ACCEPT_HPP
#define ACCEPT_HPP

#include <sys/socket.h> // accept,socklen_t

int AcceptNewClient(int server_fd, struct sockaddr *sock_addr, socklen_t *addrlen);

#endif /* ACCEPT_HPP */
