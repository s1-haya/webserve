
#include <cstring>
#include <iostream>
#include <netdb.h>      // getaddrinfo,freeaddrinfo
#include <netinet/in.h> // struct sockaddr_in
#include <stdexcept>
#include <sys/socket.h> // socket
#include <sys/types.h>
#include <unistd.h> // close
#include <vector>

#define SYSTEM_ERROR (-1)

void Connect(const std::string &host, const std::string &port) {
	struct addrinfo hints, *res, *ptr;
	int             status;

	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;              // TCPストリームソケット
	hints.ai_family   = AF_UNSPEC;                // Allows IPv4 or IPv6
	hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV; // 自動的に自分の IP を設定, IPv4・IPv6に対応

	if ((status = getaddrinfo(host.c_str(), port.c_str(), &hints, &res)) != 0) {
		std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
		return;
	}

	for (ptr = res; ptr != NULL; ptr = ptr->ai_next) {
		int sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (sockfd == -1) {
			std::cerr << "socket error" << std::endl;
			continue;
		}

		int optval = 1;
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
			std::cerr << "setsockopt error" << std::endl;
			close(sockfd);
			continue;
		}

		if (bind(sockfd, ptr->ai_addr, ptr->ai_addrlen) == -1) {
			std::cerr << "bind error" << std::endl;
			close(sockfd);
			continue;
		}

		if (listen(sockfd, 3) == SYSTEM_ERROR) {
			throw std::runtime_error("listen failed");
		}
		std::cout << "listening. fd : " << sockfd << ", host : " << host << std::endl;
		break; // 成功したらループを抜ける
	}
	freeaddrinfo(res);
}

int main() {
	Connect("localhost", "8080"); // 通常は 127.0.0.1 に解決される
	Connect("::1", "8080");       // IPv6のループバックアドレス
	// `ip addr show`にあるローカルネットワーク内のプライベートIPv4アドレスでもok

	return 0;
}
