#include "integration_client.hpp"
#include <arpa/inet.h> // inet_pton,htons
#include <cstring>     // strerror
#include <iostream>
#include <sys/socket.h> // socket,connect
#include <sys/types.h>
#include <unistd.h> // read,close

Client::Client(unsigned int port) : port_(port) {
	Init();
}

Client::~Client() {
	if (sock_fd_ != SYSTEM_ERROR) {
		close(sock_fd_);
	}
}

std::string Client::SendRequestAndReceiveResponse(const std::string &message) {
	// send request message
	send(sock_fd_, message.c_str(), message.size(), 0);

	// receive response
	char        buffer[BUFFER_SIZE];
	std::string response;
	while (true) {
		ssize_t read_ret = recv(sock_fd_, buffer, BUFFER_SIZE, 0);
		if (read_ret == SYSTEM_ERROR) {
			throw std::runtime_error(strerror(errno));
		}
		if (read_ret == 0) {
			break;
		}
		response.append(std::string(buffer, read_ret));
	}
	return response;
}

void Client::Init() {
	// socket
	if ((sock_fd_ = socket(AF_INET, SOCK_STREAM, 0)) == SYSTEM_ERROR) {
		throw std::runtime_error("socket failed");
	}
	sock_addr_.sin_family = AF_INET;
	sock_addr_.sin_port   = htons(port_);

	// convert address
	if (inet_pton(AF_INET, "127.0.0.1", &sock_addr_.sin_addr) <= 0) {
		throw std::runtime_error("inet_pton failed. Invalid address / Address not supported");
	}

	// connect sock_fd & sock_addr_
	if (connect(sock_fd_, (struct sockaddr *)&sock_addr_, sizeof(sock_addr_)) == SYSTEM_ERROR) {
		throw std::runtime_error("connect failed");
	}
}
