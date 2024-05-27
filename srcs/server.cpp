#include "server.hpp"
#include <arpa/inet.h> // htons
#include <errno.h>
#include <sys/socket.h> // socket
#include <unistd.h>     // close

// todo: set ConfigData -> private variables
Server::Server(const Config::ConfigData &config)
	: server_name_("from_config"), port_(8080) {
	Debug("server", "init server & set config");
	(void)config;
	Init();
}

Server::~Server() {
	// todo: error handle
	close(server_fd_);
}

void Server::Run() {
	Debug("server", "run server");

	struct epoll_event evlist[kMaxEvents];
	while (true) {
		errno           = 0;
		const int ready = epoll_wait(epoll_fd_, evlist, kMaxEvents, -1);
		if (ready == kSystemErr) {
			if (errno == EINTR) {
				continue;
			}
			throw std::runtime_error("epoll_wait failed");
		}
		for (std::size_t i = 0; i < (std::size_t)ready; ++i) {
			if (evlist[i].data.fd == server_fd_) {
				// accept
				const int new_socket =
					accept(server_fd_, (struct sockaddr *)&sock_addr_, &addrlen_);
				if (new_socket == kSystemErr) {
					throw std::runtime_error("accept failed");
				}
				ev_.events  = EPOLLIN;
				ev_.data.fd = new_socket;
				if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, new_socket, &ev_) ==
					kSystemErr) {
					throw std::runtime_error("epoll_ctl failed");
				}
			} else {
				// read,send
				const int client_fd = evlist[i].data.fd;

				char    buffer[kBufferSize];
				ssize_t read_ret = read(client_fd, buffer, kBufferSize);
				if (read_ret <= 0) {
					close(client_fd);
					epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, client_fd, NULL);
				} else {
					send(client_fd, buffer, read_ret, 0);
				}
			}
		}
	}
}

void Server::Init() {
	addrlen_ = sizeof(sock_addr_);

	// socket
	server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd_ == kSystemErr) {
		throw std::runtime_error("socket failed");
	}
	sock_addr_.sin_family      = AF_INET;
	sock_addr_.sin_addr.s_addr = INADDR_ANY;
	sock_addr_.sin_port        = htons(port_);

	// bind
	if (bind(server_fd_, (const struct sockaddr *)&sock_addr_, addrlen_) ==
		kSystemErr) {
		throw std::runtime_error("bind failed");
	}

	// listen
	if (listen(server_fd_, 3) == kSystemErr) {
		throw std::runtime_error("listen failed");
	}

	// create epoll
	epoll_fd_ = epoll_create1(EPOLL_CLOEXEC);
	if (epoll_fd_ == kSystemErr) {
		throw std::runtime_error("epoll_create failed");
	}
	ev_.events  = EPOLLIN;
	ev_.data.fd = server_fd_;
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, server_fd_, &ev_) == kSystemErr) {
		throw std::runtime_error("epoll_ctl failed");
	}
}
