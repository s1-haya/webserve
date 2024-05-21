#ifndef BIND_HPP
#define BIND_HPP

#include <sys/socket.h> // bind,socklen_t

bool BindSocket(int server_fd, const struct sockaddr *addr, socklen_t addrlen);

#endif /* BIND_HPP */
