#include "client.hpp"
#include "color.hpp"
#include "utils.hpp"
#include <arpa/inet.h> // inet_pton,htons
#include <cerrno>
#include <cstring> // strerror
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

namespace {

void DebugPrint(const std::string &s) {
	std::cerr << utils::color::GRAY << s << utils::color::RESET << std::endl;
}

} // namespace

void Client::SendRequestAndReceiveResponse(const std::string &message) {
	// send request message
	ssize_t send_size = send(sock_fd_, message.c_str(), message.size(), 0);
	DebugPrint("Message sent. (" + utils::ToString(send_size) + " bytes)");

	DebugPrint("Response from server:");
	// receive response
	char buffer[BUFFER_SIZE];
	while (true) {
		errno            = 0;
		ssize_t read_ret = read(sock_fd_, buffer, BUFFER_SIZE);
		if (read_ret == SYSTEM_ERROR) {
			if (errno == ECONNRESET) {
				// Connection reset by peer
				break;
			}
			throw std::runtime_error("read failed" + std::string(std::strerror(errno)));
		}
		if (read_ret == 0) {
			break;
		}
		const std::string response = std::string(buffer, read_ret);
		std::cout << response << std::endl;
	}
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
