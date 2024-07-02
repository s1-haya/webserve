#include "server.hpp"
#include "connection.hpp"
#include "event.hpp"
#include "http.hpp"
#include "utils.hpp"
#include <errno.h>
#include <sys/socket.h> // send
#include <unistd.h>     // close

namespace server {

// todo: set ConfigData -> private variables
Server::Server(const _config::Config::ConfigData &config) {
	(void)config;

	std::vector<TempConfig> servers;
	servers.push_back(std::make_pair("localhost", 8080));
	servers.push_back(std::make_pair("abcde", 12345));
	Init(servers);
}

Server::~Server() {
	sock_infos_.clear();
}

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

bool IsServerFd(int sock_fd, const Server::SockInfos &sock_infos) {
	return sock_infos.count(sock_fd) == 1;
}

} // namespace

void Server::HandleEvent(const event::Event &event) {
	SockInfo *sock_info = static_cast<SockInfo *>(event.sock_info);
	if (IsServerFd(sock_info->fd, sock_infos_)) {
		HandleNewConnection(event);
	} else {
		HandleExistingConnection(event);
	}
}

void Server::HandleNewConnection(const event::Event &event) {
	SockInfo *sock_info = static_cast<SockInfo *>(event.sock_info);

	const int client_fd = Connection::Accept(sock_info->fd);
	if (client_fd == SYSTEM_ERROR) {
		throw std::runtime_error("accept failed");
	}
	// todo: new なくせないか
	// SockInfo client        = SockInfo(sock_info->name, client_fd);
	// sock_infos_[client_fd] = client;
	// monitor_.AddNewConnection(&sock_infos_[client_fd], event::EVENT_READ);
	monitor_.AddNewConnection(new SockInfo(sock_info->name, client_fd), event::EVENT_READ);
	utils::Debug("server", "add new client", client_fd);
}

void Server::HandleExistingConnection(const event::Event &event) {
	if (event.type & event::EVENT_READ) {
		ReadRequest(event);
	}
	if (event.type & event::EVENT_WRITE) {
		SendResponse(event);
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
	SockInfo *sock_info = static_cast<SockInfo *>(event.sock_info);
	const int client_fd = sock_info->fd;

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
		monitor_.UpdateEventType(client_fd, sock_info, event::EVENT_WRITE);
	}
}

void Server::SendResponse(const event::Event &event) {
	SockInfo *sock_info = static_cast<SockInfo *>(event.sock_info);
	const int client_fd = sock_info->fd;

	// todo: check if it's ready to start write/send
	const std::string response = CreateHttpResponse(buffers_.GetBuffer(client_fd));
	send(client_fd, response.c_str(), response.size(), 0);
	utils::Debug("server", "send response to client", client_fd);

	// disconnect
	buffers_.Delete(client_fd);
	close(client_fd);
	monitor_.DeleteConnection(sock_info);
	sock_infos_.erase(client_fd);
	utils::Debug("server", "disconnected client", client_fd);
	utils::Debug("------------------------------------------");
}

void Server::Init(const std::vector<TempConfig> &servers) {
	typedef std::vector<TempConfig>::const_iterator Itr;
	for (Itr it = servers.begin(); it != servers.end(); ++it) {
		const std::string  server_name = it->first;
		const unsigned int port        = it->second;

		const unsigned int fd     = Connection::Init(port);
		SockInfo           server = SockInfo(server_name, fd);
		sock_infos_[server.fd]    = server;
		// add to epoll's interest list
		monitor_.AddNewConnection(&sock_infos_[server.fd], event::EVENT_READ);
		utils::Debug("server", "init server & listen", server.fd);
	}
}

} // namespace server
