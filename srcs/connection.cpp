#include "connection.hpp"
#include "server.hpp"
#include <netinet/in.h> // struct sockaddr_in
#include <stdexcept>    // runtime_error
#include <sys/socket.h> // socket

// todo: tmp
#include "utils.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>

namespace server {

// todo: port->uint16_t
int Connection::Init(unsigned int port) {
	// socket
	const int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == SYSTEM_ERROR) {
		throw std::runtime_error("socket failed");
	}
	// set socket option to reuse address
	int optval = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == SYSTEM_ERROR) {
		throw std::runtime_error("setsockopt failed");
	}

	struct sockaddr_in sock_addr;
	sock_addr.sin_family      = AF_INET;
	sock_addr.sin_addr.s_addr = INADDR_ANY;
	sock_addr.sin_port        = htons(port);
	const socklen_t addr_len  = sizeof(sock_addr);
	// bind
	if (bind(server_fd, (const struct sockaddr *)&sock_addr, addr_len) == SYSTEM_ERROR) {
		throw std::runtime_error("bind failed");
	}

	// listen
	if (listen(server_fd, 3) == SYSTEM_ERROR) {
		throw std::runtime_error("listen failed");
	}
	return server_fd;
}

int Connection::Accept(int server_fd) {
	struct sockaddr_in client_sock_addr;
	socklen_t          addrlen = sizeof(client_sock_addr);
	const int new_socket       = accept(server_fd, (struct sockaddr *)&client_sock_addr, &addrlen);
	// todo
	// if (new_socket == SYSTEM_ERROR) {
	// 	throw std::runtime_error("accept failed");
	// }

	// clientのIP addressとportを取得できる
	// todo: getaddrinfo() 使う
	char client_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(client_sock_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
	const int client_port = ntohs(client_sock_addr.sin_port);
	utils::Debug("accept", "client IP: " + std::string(client_ip) + " / port", client_port);

	return new_socket;
}

} // namespace server
