#include "server.hpp"
#include "client_info.hpp"
#include "event.hpp"
#include "http.hpp"
#include "server_info.hpp"
#include "utils.hpp"
#include "virtual_server.hpp"
#include <errno.h>
#include <sys/socket.h> // socket
#include <unistd.h>     // close

namespace server {
namespace {

// todo: tmp
std::list<TempConfig> CreateTempConfig() {
	std::list<TempConfig> all_configs;

	// virtual server 1
	TempConfig config1;
	config1.server_name = "localhost";
	config1.locations.push_back("/www/");
	config1.ports.push_back("8080");
	config1.ports.push_back("12345");
	all_configs.push_back(config1);

	// virtual server 2
	TempConfig config2;
	config2.server_name = "test_serv";
	config2.locations.push_back("/");
	config2.locations.push_back("/static/");
	config2.ports.push_back("9999");
	all_configs.push_back(config2);

	return all_configs;
}

VirtualServer::PortList ConvertPorts(const std::list<std::string> &ports_str) {
	VirtualServer::PortList port_list;

	typedef std::list<std::string>::const_iterator It;
	for (It it = ports_str.begin(); it != ports_str.end(); ++it) {
		unsigned int port;
		if (!utils::ConvertStrToUint(*it, port)) {
			// todo: original exception
			throw std::logic_error("wrong port number");
		}
		port_list.push_back(port);
	}
	return port_list;
}

} // namespace

void Server::AddVirtualServers(const Server::TempAllConfig &all_configs) {
	typedef Server::TempAllConfig::const_iterator Itr;
	for (Itr it = all_configs.begin(); it != all_configs.end(); ++it) {
		const TempConfig &config = *it;

		// todo: validate server_name, port, etc?
		VirtualServer::PortList ports = ConvertPorts(config.ports);
		VirtualServer           virtual_server(config.server_name, config.locations, ports);
		virtual_servers_.AddVirtualServer(virtual_server);
	}
}

// todo: set ConfigData -> private variables
Server::Server(const _config::Config::ConfigData &config) {
	(void)config;

	// todo: tmp
	TempAllConfig all_configs = CreateTempConfig();
	AddVirtualServers(all_configs);
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
	// todo: return accept error like Result
	const ClientInfo new_client_info = Connection::Accept(server_fd);
	const int        client_fd       = new_client_info.GetFd();
	if (client_fd == SYSTEM_ERROR) {
		throw std::runtime_error("accept failed");
	}

	// add to context
	context_.AddClientInfo(client_fd, new_client_info, server_fd);
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

std::string Server::CreateHttpResponse(int client_fd) const {
	const std::string &request_buf = buffers_.GetBuffer(client_fd);

	http::Http http(request_buf);
	// todo: parse?
	// todo: tmp
	const bool is_cgi = true;
	if (is_cgi) {
		// use client_info -> get client_ip
		const ClientInfo  &client_info = context_.GetClientInfo(client_fd);
		const std::string &client_ip   = client_info.GetIp();

		// use server_info -> get server_fd, server_port
		const ServerInfo  &server_info = context_.GetConnectedServerInfo(client_fd);
		const int          server_fd   = server_info.GetFd();
		const std::string &server_port = utils::ConvertUintToStr(server_info.GetPort());

		// use virtual_server -> get server_name, locations
		const VirtualServer &virtual_server          = virtual_servers_.GetVirtualServer(server_fd);
		const std::string   &server_name             = virtual_server.GetServerName();
		const VirtualServer::LocationList &locations = virtual_server.GetLocations();
		utils::Debug(
			"server",
			"ClientInfo - IP: " + client_ip + " / ServerInfo - name: " + server_name +
				", location: " + utils::FormatListToStr(locations) + ", port: " + server_port +
				", fd",
			server_fd
		);
		// todo: call cgi(client_info, server_info)?
	}
	return http.CreateResponse();
}

void Server::SendResponse(int client_fd) {
	// todo: check if it's ready to start write/send
	const std::string response = CreateHttpResponse(client_fd);
	send(client_fd, response.c_str(), response.size(), 0);
	utils::Debug("server", "send response to client", client_fd);

	// disconnect
	buffers_.Delete(client_fd);
	context_.DeleteClientInfo(client_fd);
	event_monitor_.Delete(client_fd);
	close(client_fd);
	utils::Debug("server", "disconnected client", client_fd);
	utils::Debug("------------------------------------------");
}

void Server::Init() {
	const VirtualServerStorage::VirtualServerList &all_virtual_server_list =
		virtual_servers_.GetAllVirtualServerList();

	typedef VirtualServerStorage::VirtualServerList::const_iterator ItVirtualServer;
	for (ItVirtualServer it = all_virtual_server_list.begin(); it != all_virtual_server_list.end();
		 ++it) {
		const VirtualServer           &virtual_server = *it;
		const VirtualServer::PortList &ports          = virtual_server.GetPorts();

		// 各virtual serverの全portをsocket通信
		typedef VirtualServer::PortList::const_iterator ItPort;
		for (ItPort it_port = ports.begin(); it_port != ports.end(); ++it_port) {
			// create ServerInfo
			ServerInfo server_info(*it_port);
			const int  server_fd = connection_.Connect(server_info);
			server_info.SetSockFd(server_fd);

			// add to context
			context_.AddServerInfo(server_fd, server_info);
			virtual_servers_.AddMapping(server_fd, &virtual_server);
			event_monitor_.Add(server_fd, event::EVENT_READ);
			utils::Debug("server", "listen", server_fd);
		}
	}
}

} // namespace server
