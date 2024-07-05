#include "connection.hpp"
#include "server.hpp"
#include <stdexcept>    // runtime_error
#include <sys/socket.h> // socket,setsockopt,bind,listen,accept

namespace server {

int Connection::Init(struct sockaddr_in &sock_addr, socklen_t addrlen) {
	// socket
	const int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == SYSTEM_ERROR) {
		throw std::runtime_error("socket failed");
	}
	// set socket option to reuse address
	// optval : Specify a non-zero value to enable the boolean option, or zero to disable it
	int optval = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == SYSTEM_ERROR) {
		throw std::runtime_error("setsockopt failed");
	}

	// bind
	if (bind(server_fd, (const struct sockaddr *)&sock_addr, addrlen) == SYSTEM_ERROR) {
		throw std::runtime_error("bind failed");
	}

	// listen
	if (listen(server_fd, 3) == SYSTEM_ERROR) {
		throw std::runtime_error("listen failed");
	}
	return server_fd;
}

int Connection::Accept(int server_fd, struct sockaddr_in &sock_addr, socklen_t *addrlen) {
	const int new_socket = accept(server_fd, (struct sockaddr *)&sock_addr, addrlen);
	// todo: need?
	// if (new_socket == SYSTEM_ERROR) {
	// 	throw std::runtime_error("accept failed");
	// }
	return new_socket;
}

} // namespace server
