#include "server.hpp"
#include "event.hpp"
#include "http.hpp"
#include <arpa/inet.h> // htons
#include <errno.h>
#include <sys/socket.h> // socket
#include <unistd.h>     // close

// todo: set ConfigData -> private variables
Server::Server(const Config::ConfigData &config)
	: server_name_("from_config"), port_(8080) {
	(void)config;
	Init();
	Debug("server", "init server & listen", server_fd_);
}

Server::~Server() {
	// todo: close() error handle
	if (server_fd_ != SYSTEM_ERROR) {
		close(server_fd_);
	}
}

void Server::Run() {
	Debug("server", "run server", server_fd_);

	while (true) {
		errno           = 0;
		const int ready = epoll_.CreateReadyList();
		// todo: error handle
		if (ready == SYSTEM_ERROR && errno == EINTR) {
			continue;
		}
		for (std::size_t i = 0; i < static_cast<std::size_t>(ready); ++i) {
			HandleEvent(epoll_.GetEvent(i));
		}
	}
}

void Server::HandleEvent(const Event &event) {
	if (event.fd == server_fd_) {
		AcceptNewConnection();
		return;
	}
	if (event.type & EVENT_READ) {
		SendResponse(event.fd);
	}
	if (event.type & EVENT_WRITE) {
		// todo
	}
	// todo: handle other EventType (switch)
}

void Server::AcceptNewConnection() {
	const int new_socket =
		accept(server_fd_, (struct sockaddr *)&sock_addr_, &addrlen_);
	if (new_socket == SYSTEM_ERROR) {
		throw std::runtime_error("accept failed");
	}
	epoll_.AddNewConnection(new_socket, EVENT_READ);
	Debug("server", "add new client", new_socket);
}

namespace {
	std::string CreateHttpResponse(const std::string &read_buf) {
		Http http(read_buf);
		return http.CreateResponse();
	}
} // namespace

void Server::SendResponse(int client_fd) {
	ssize_t read_ret = buffers_.Read(client_fd);
	if (read_ret <= 0) {
		if (read_ret == SYSTEM_ERROR) {
			throw std::runtime_error("read failed");
		}
		buffers_.Delete(client_fd);
		close(client_fd);
		epoll_.DeleteConnection(client_fd);
		Debug("server", "disconnected client", client_fd);
		return;
	}
	const std::string response = CreateHttpResponse(buffers_.GetBuffer(client_fd));
	send(client_fd, response.c_str(), response.size(), 0);
	Debug("server", "send response to client", client_fd);
}

void Server::Init() {
	addrlen_ = sizeof(sock_addr_);

	// socket
	server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd_ == SYSTEM_ERROR) {
		throw std::runtime_error("socket failed");
	}
	// set socket option to reuse address
	int optval = 1;
	if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) ==
		SYSTEM_ERROR) {
		throw std::runtime_error("setsockopt failed");
	}
	sock_addr_.sin_family      = AF_INET;
	sock_addr_.sin_addr.s_addr = INADDR_ANY;
	sock_addr_.sin_port        = htons(port_);

	// bind
	if (bind(server_fd_, (const struct sockaddr *)&sock_addr_, addrlen_) ==
		SYSTEM_ERROR) {
		throw std::runtime_error("bind failed");
	}

	// listen
	if (listen(server_fd_, 3) == SYSTEM_ERROR) {
		throw std::runtime_error("listen failed");
	}

	// add to epoll's interest list
	epoll_.AddNewConnection(server_fd_, EVENT_READ);
}
