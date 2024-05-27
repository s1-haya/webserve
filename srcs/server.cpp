#include "server.hpp"
#include "string.hpp"
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
	// todo: close() error handle
	if (server_fd_ != kSystemErr) {
		close(server_fd_);
	}
}

void Server::Run() {
	Debug("server", "run server");

	while (true) {
		errno           = 0;
		const int ready = epoll_.CreateReadyList();
		// todo: error handle
		if (ready == kSystemErr && errno == EINTR) {
			continue;
		}
		for (std::size_t i = 0; i < static_cast<std::size_t>(ready); ++i) {
			HandleEvent(epoll_.GetEvent(i));
		}
	}
}

void Server::HandleEvent(const struct epoll_event &ev) {
	if (ev.data.fd == server_fd_) {
		// accept
		const int new_socket =
			accept(server_fd_, (struct sockaddr *)&sock_addr_, &addrlen_);
		if (new_socket == kSystemErr) {
			throw std::runtime_error("accept failed");
		}
		epoll_.AddNewConnection(new_socket);
		Debug("server", "add new client (fd: " + ToString(new_socket) + ")");
	} else if (ev.events & EPOLLIN) {
		// read,send
		const int client_fd = ev.data.fd;

		char    buffer[kBufferSize];
		ssize_t read_ret = read(client_fd, buffer, kBufferSize);
		if (read_ret <= 0) {
			if (read_ret == kSystemErr) {
				throw std::runtime_error("read failed");
			}
			Debug("server", "disconnected client (fd: " + ToString(client_fd) + ")");
			close(client_fd);
			epoll_.DeleteConnection(client_fd);
		} else {
			send(client_fd, buffer, read_ret, 0);
			Debug("server", "send to client (fd: " + ToString(client_fd) + ")");
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

	// add to epoll's interest list
	epoll_.AddNewConnection(server_fd_);
}
