#include "server.hpp"
#include "connection.hpp"
#include "event.hpp"
#include "http.hpp"
#include "utils.hpp"
#include <arpa/inet.h> // htons
#include <errno.h>
#include <sys/socket.h> // socket
#include <unistd.h>     // close

namespace server {

// todo: set ConfigData -> private variables
Server::Server(const _config::Config::ConfigData &config)
	: server_name_("from_config"), port_(8080) {
	(void)config;
	Init();
	utils::Debug("server", "init server & listen", server_fd_);
}

Server::~Server() {
	// todo: close() error handle
	if (server_fd_ != SYSTEM_ERROR) {
		close(server_fd_);
	}
}

void Server::Run() {
	utils::Debug("server", "run server", server_fd_);

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

void Server::HandleEvent(const event::Event &event) {
	if (event.fd == server_fd_) {
		HandleNewConnection();
	} else {
		HandleExistingConnection(event);
	}
}

void Server::HandleNewConnection() {
	// A new socket that has established a connection with the peer socket.
	const int new_sock_fd = Connection::Accept(server_fd_, sock_addr_, &addrlen_);
	if (new_sock_fd == SYSTEM_ERROR) {
		throw std::runtime_error("accept failed");
	}
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
	close(client_fd);
	monitor_.DeleteConnection(client_fd);
	utils::Debug("server", "disconnected client", client_fd);
	utils::Debug("------------------------------------------");
}

void Server::Init() {
	sock_addr_.sin_family      = AF_INET;
	sock_addr_.sin_addr.s_addr = INADDR_ANY;
	sock_addr_.sin_port        = htons(port_);
	addrlen_                   = sizeof(sock_addr_);

	server_fd_ = Connection::Init(sock_addr_, addrlen_);
	// add to epoll's interest list
	monitor_.AddNewConnection(server_fd_, event::EVENT_READ);
}

} // namespace server
