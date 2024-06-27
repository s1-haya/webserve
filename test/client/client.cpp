#include "client.hpp"
#include <arpa/inet.h> // inet_pton,htons
#include <iostream>
#include <sys/socket.h> // socket,connect
#include <sys/types.h>
#include <unistd.h> // read,close

const std::string Client::DEFAULT_MESSAGE = "Hello from client (default)";

Client::Client() : port_(DEFAULT_PORT), request_message_(DEFAULT_MESSAGE) {
	Init();
}

Client::Client(int port) : port_(port), request_message_(DEFAULT_MESSAGE) {
	Init();
}

Client::Client(const std::string &message) : port_(DEFAULT_PORT), request_message_(message) {
	Init();
}

Client::Client(int port, const std::string &message) : port_(port), request_message_(message) {
	Init();
}

Client::~Client() {
	if (sock_fd_ != SYSTEM_ERROR) {
		close(sock_fd_);
	}
}

namespace {

static const std::string COLOR_GRAY  = "\033[30m";
static const std::string COLOR_RESET = "\033[0m";

void DebugPrint(const std::string &s) {
	std::cerr << COLOR_GRAY << s << COLOR_RESET << std::endl;
}

} // namespace

void Client::SendRequestAndReceiveResponse(const std::string &message) {
	// send request message
	send(sock_fd_, message.c_str(), message.size(), 0);
	DebugPrint("Message sent.");

	// receive response
	char        buffer[BUFFER_SIZE];
	std::string response;
	while (true) {
		ssize_t read_ret = read(sock_fd_, buffer, BUFFER_SIZE);
		if (read_ret == SYSTEM_ERROR) {
			throw std::runtime_error("read failed");
		}
		if (read_ret == 0) {
			break;
		}
		response.append(std::string(buffer, read_ret));
	}
	DebugPrint("Response from server:");
	std::cout << response << std::endl;
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
