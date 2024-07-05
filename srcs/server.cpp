#include "server.hpp"
#include "connection.hpp"
#include "event.hpp"
#include "http.hpp"
#include "sock_info.hpp"
#include "utils.hpp"
#include <errno.h>
#include <sys/socket.h> // socket
#include <unistd.h>     // close

namespace server {

// todo: set ConfigData -> private variables
Server::Server(const _config::Config::ConfigData &config) {
	(void)config;
	Init("localhost", 8080);
}

Server::~Server() {}

void Server::Run() {
	utils::Debug("server", "run server");

	while (true) {
		errno           = 0;
		const int ready = monitor_.CreateReadyList();
		// todo: error handle
		if (ready == SYSTEM_ERROR && errno == EINTR) {
			continue;
		}
		for (std::size_t i = 0; i < static_cast<std::size_t>(ready); ++i) {
			HandleEvent(monitor_.GetEvent(i));
		}
	}
}

namespace {

bool IsListenServerFd(int sock_fd, const Server::FdSet &listen_server_fds) {
	return listen_server_fds.count(sock_fd) == 1;
}

} // namespace

void Server::HandleEvent(const event::Event &event) {
	const int sock_fd = event.fd;
	if (IsListenServerFd(sock_fd, listen_server_fds_)) {
		HandleNewConnection(sock_fd);
	} else {
		HandleExistingConnection(event);
	}
}

void Server::HandleNewConnection(int sock_fd) {
	SockInfo &sock_info = context_.GetSockInfo(sock_fd);

	// A new socket that has established a connection with the peer socket.
	const int new_sock_fd = Connection::Accept(sock_info);
	if (new_sock_fd == SYSTEM_ERROR) {
		throw std::runtime_error("accept failed");
	}

	SockInfo new_sock_info = sock_info;
	new_sock_info.SetPeerSockFd(new_sock_fd);
	// add to context
	context_.AddSockInfo(new_sock_fd, new_sock_info);
	// add to epoll's interest list
	monitor_.AddNewConnection(new_sock_fd, event::EVENT_READ);
	utils::Debug("server", "add new client", new_sock_fd);
}

void Server::HandleExistingConnection(const event::Event &event) {
	if (event.type & event::EVENT_READ) {
		ReadRequest(event);
	}
	if (event.type & event::EVENT_WRITE) {
		SendResponse(event.fd);
	}
	// todo: handle other EventType
}

namespace {

std::string CreateHttpResponse(const std::string &read_buf) {
	http::Http http(read_buf);
	return http.CreateResponse();
}

// todo: find "Connection: close"?
bool IsRequestReceivedComplete(const std::string &buffer) {
	return buffer.find("\r\n\r\n") != std::string::npos;
}

} // namespace

void Server::ReadRequest(const event::Event &event) {
	const int client_fd = event.fd;

	ssize_t read_ret = buffers_.Read(client_fd);
	if (read_ret <= 0) {
		if (read_ret == SYSTEM_ERROR) {
			throw std::runtime_error("read failed");
		}
		// todo: need?
		// buffers_.Delete(client_fd);
		// monitor_.DeleteConnection(client_fd);
		return;
	}
	if (IsRequestReceivedComplete(buffers_.GetBuffer(client_fd))) {
		utils::Debug("server", "received all request from client", client_fd);
		std::cerr << buffers_.GetBuffer(client_fd) << std::endl;
		monitor_.UpdateEventType(event, event::EVENT_WRITE);
	}
}

void Server::SendResponse(int client_fd) {
	// todo: check if it's ready to start write/send
	const std::string response = CreateHttpResponse(buffers_.GetBuffer(client_fd));
	send(client_fd, response.c_str(), response.size(), 0);
	utils::Debug("server", "send response to client", client_fd);

	// disconnect
	buffers_.Delete(client_fd);
	context_.DeleteSockInfo(client_fd);
	monitor_.DeleteConnection(client_fd);
	close(client_fd);
	utils::Debug("server", "disconnected client", client_fd);
	utils::Debug("------------------------------------------");
}

void Server::Init(const std::string &server_name, unsigned int port) {
	SockInfo server_sock_info(server_name, port);
	// connect & listen
	const int server_fd = Connection::Init(server_sock_info);
	server_sock_info.SetSockFd(server_fd);

	listen_server_fds_.insert(server_fd);
	// add to context
	context_.AddSockInfo(server_fd, server_sock_info);
	// add to epoll's interest list
	monitor_.AddNewConnection(server_fd, event::EVENT_READ);
	utils::Debug("server", "init server & listen", server_fd);
}

} // namespace server
