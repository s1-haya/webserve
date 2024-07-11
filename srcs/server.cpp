#include "server.hpp"
#include "event.hpp"
#include "http.hpp"
#include "sock_info.hpp"
#include "utils.hpp"
#include <errno.h>
#include <sys/socket.h> // socket
#include <unistd.h>     // close

namespace server {
namespace {

Server::SockInfoVec ConvertConfigToSockInfoVec(std::vector<Server::TempConfig> server_configs) {
	Server::SockInfoVec sock_infos;

	typedef std::vector<Server::TempConfig>::const_iterator Itr;
	for (Itr it = server_configs.begin(); it != server_configs.end(); ++it) {
		const std::string &server_name = it->first;
		unsigned int       port;
		if (!utils::ConvertStrToUint(it->second, port)) {
			// todo: original exception
			throw std::logic_error("wrong port number");
		}

		// todo: validate server_name, port, etc?
		SockInfo server_sock_info(server_name, port);
		sock_infos.push_back(server_sock_info);
	}
	return sock_infos;
}

} // namespace

// todo: set ConfigData -> private variables
Server::Server(const _config::Config::ConfigData &config) {
	(void)config;

	// todo: tmp
	std::vector<TempConfig> server_configs;
	server_configs.push_back(std::make_pair("0.0.0.0", "8080"));
	// server_configs.push_back(std::make_pair("localhost", "12345"));
	// server_configs.push_back(std::make_pair("::1", "8080"));

	SockInfoVec sock_infos = ConvertConfigToSockInfoVec(server_configs);
	Init(sock_infos);
}
Server::~Server() {}

void Server::Run() {
	utils::Debug("server", "run server");

	while (true) {
		errno           = 0;
		const int ready = event_monitor_.CreateReadyList();
		// todo: error handle
		if (ready == SYSTEM_ERROR && errno == EINTR) {
			continue;
		}
		for (std::size_t i = 0; i < static_cast<std::size_t>(ready); ++i) {
			HandleEvent(event_monitor_.GetEvent(i));
		}
	}
}

void Server::HandleEvent(const event::Event &event) {
	const int sock_fd = event.fd;
	if (connection_.IsListenServerFd(sock_fd)) {
		HandleNewConnection(sock_fd);
	} else {
		HandleExistingConnection(event);
	}
}

void Server::HandleNewConnection(int server_fd) {
	// A new socket that has established a connection with the peer socket.
	const int client_fd = Connection::Accept(server_fd);
	if (client_fd == SYSTEM_ERROR) {
		throw std::runtime_error("accept failed");
	}

	SockInfo new_sock_info;
	new_sock_info.SetPeerSockFd(client_fd);
	// add to context
	context_.AddSockInfo(client_fd, new_sock_info);
	event_monitor_.Add(client_fd, event::EVENT_READ);
	utils::Debug("server", "add new client", client_fd);
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
		// event_monitor_.Delete(client_fd);
		return;
	}
	if (IsRequestReceivedComplete(buffers_.GetBuffer(client_fd))) {
		utils::Debug("server", "received all request from client", client_fd);
		std::cerr << buffers_.GetBuffer(client_fd) << std::endl;
		event_monitor_.Update(event, event::EVENT_WRITE);
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
	event_monitor_.Delete(client_fd);
	close(client_fd);
	utils::Debug("server", "disconnected client", client_fd);
	utils::Debug("------------------------------------------");
}

void Server::Init(const SockInfoVec &sock_infos) {
	typedef SockInfoVec::const_iterator Itr;
	for (Itr it = sock_infos.begin(); it != sock_infos.end(); ++it) {
		SockInfo server_sock_info = *it;
		// connect & listen
		const int server_fd = connection_.Connect(server_sock_info);
		server_sock_info.SetSockFd(server_fd);

		// add to context
		context_.AddSockInfo(server_fd, server_sock_info);
		event_monitor_.Add(server_fd, event::EVENT_READ);
		utils::Debug("server", "init server & listen", server_fd);
	}
}

} // namespace server
