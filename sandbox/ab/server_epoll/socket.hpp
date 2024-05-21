#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netinet/in.h> // struct sockaddr_in
#include <sys/socket.h> // socket

int  CreateSocket();
void SetSocketAddr(struct sockaddr_in &sock_addr);

#endif /* SOCKET_HPP */
