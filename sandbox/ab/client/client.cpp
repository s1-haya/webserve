#include <arpa/inet.h> // inet_pton,htons
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>      // perror
#include <sys/socket.h> // socket,connect
#include <unistd.h>     // read,close

#define PORT         8080
#define BUFFER_SIZE  1024
#define SYSTEM_ERROR (-1)

int main() {
	int                sock_fd = 0;
	struct sockaddr_in sock_addr;
	char               buffer[BUFFER_SIZE];
	std::string        message = "Hello from client";

	// socket
	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == SYSTEM_ERROR) {
		perror("socket failed");
		return EXIT_FAILURE;
	}
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port   = htons(PORT);

	// 指定された可読形式のIPアドレスをネットワークバイトオーダIPアドレスへ変換する
	// IPアドレスが解釈不能の場合 return 0
	if (inet_pton(AF_INET, "127.0.0.1", &sock_addr.sin_addr) <= 0) {
		perror("inet_pton failed. Invalid address / Address not supported");
		return EXIT_FAILURE;
	}

	// sockfdに指定されたアクティブソケットを、addrおよびaddrlenに指定されたアドレスのリスニングソケットへ接続
	if (connect(sock_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) ==
		SYSTEM_ERROR) {
		perror("connect failed");
		return EXIT_FAILURE;
	}

	send(sock_fd, message.c_str(), message.length(), 0);
	std::cout << "Message sent" << std::endl;

	ssize_t read_ret = read(sock_fd, buffer, BUFFER_SIZE);
	std::cout << "Echo from server: " << std::string(buffer, read_ret) << std::endl;

	close(sock_fd);
	return EXIT_SUCCESS;
}
