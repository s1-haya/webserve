#include "server.hpp"
#include "client_info.hpp"
#include "dto_server_to_http.hpp"
#include "event.hpp"
#include "server_info.hpp"
#include "utils.hpp"
#include "virtual_server.hpp"
#include <errno.h>
#include <sys/socket.h> // socket
#include <unistd.h>     // close

namespace server {
namespace {

VirtualServer::LocationList ConvertLocations(const config::context::LocationList &config_locations
) {
	VirtualServer::LocationList location_list;

	typedef config::context::LocationList::const_iterator Itr;
	for (Itr it = config_locations.begin(); it != config_locations.end(); ++it) {
		Location location;
		location.location       = it->request_uri;
		location.root           = it->alias;
		location.index          = it->index;
		location.allowed_method = "GET"; // todo: tmp
		location_list.push_back(location);
	}
	return location_list;
}

// todo: configからstd::list<unsigned int>で渡されるようになったら変更する
VirtualServer::PortList ConvertPorts(const config::context::PortList &ports_str) {
	VirtualServer::PortList port_list;

	typedef config::context::PortList::const_iterator Itr;
	for (Itr it = ports_str.begin(); it != ports_str.end(); ++it) {
		const unsigned int port = static_cast<unsigned int>(*it);
		port_list.push_back(port);
	}
	return port_list;
}

VirtualServer ConvertToVirtualServer(const config::context::ServerCon &config_server) {
	const std::string          &server_name = *(config_server.server_names.begin()); // tmp
	VirtualServer::LocationList locations   = ConvertLocations(config_server.location_con);
	VirtualServer::PortList     ports       = ConvertPorts(config_server.port);
	return VirtualServer(server_name, locations, ports);
}

// todo: tmp for debug
void PrintLocations(const VirtualServer::LocationList &locations) {
	typedef VirtualServer::LocationList::const_iterator Itr;
	for (Itr it = locations.begin(); it != locations.end(); ++it) {
		const server::Location &location = *it;
		std::cerr << "- location: " << location.location << ", root: " << location.root
				  << ", index: " << location.index << std::endl;
	}
}

// todo: tmp for debug
void DebugDto(const DtoClientInfos &client_infos, const DtoServerInfos &server_infos) {
	utils::Debug("server", "ClientInfo - IP: " + client_infos.ip + ", fd", client_infos.fd);
	utils::Debug("server", "received ServerInfo, fd", server_infos.fd);
	std::cerr << "server_name: " << server_infos.server_name << ", port: " << server_infos.port
			  << std::endl;
	std::cerr << "locations: " << std::endl;
	PrintLocations(server_infos.locations);
}

} // namespace

void Server::AddVirtualServers(const ConfigServers &config_servers) {
	typedef ConfigServers::const_iterator Itr;
	for (Itr it = config_servers.begin(); it != config_servers.end(); ++it) {
		VirtualServer virtual_server = ConvertToVirtualServer(*it);
		context_.AddVirtualServer(virtual_server);
	}
}

Server::Server(const ConfigServers &config_servers) {
	AddVirtualServers(config_servers);
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
	const ClientInfo new_client_info = Connection::Accept(server_fd);
	const int        client_fd       = new_client_info.GetFd();

	// add to context
	context_.AddClientInfo(new_client_info, server_fd);
	event_monitor_.Add(client_fd, event::EVENT_READ);
	utils::Debug("server", "add new client", client_fd);
}

void Server::HandleExistingConnection(const event::Event &event) {
	if (event.type & event::EVENT_READ) {
		ReadRequest(event.fd);
		RunHttp(event);
	}
	if (event.type & event::EVENT_WRITE) {
		SendResponse(event.fd);
	}
	// todo: handle other EventType
}

DtoClientInfos Server::GetClientInfos(int client_fd) const {
	DtoClientInfos client_infos;
	client_infos.fd          = client_fd;
	client_infos.request_buf = buffers_.GetRequest(client_fd);
	client_infos.ip          = context_.GetClientIp(client_fd);
	return client_infos;
}

DtoServerInfos Server::GetServerInfos(int client_fd) const {
	const ServerContext &server_context = context_.GetServerContext(client_fd);

	DtoServerInfos server_infos;
	server_infos.fd          = server_context.fd;
	server_infos.server_name = server_context.server_name;
	server_infos.port        = server_context.port;
	server_infos.locations   = server_context.locations;
	return server_infos;
}

void Server::ReadRequest(int client_fd) {
	ssize_t read_ret = buffers_.ReadRequest(client_fd);
	if (read_ret <= 0) {
		if (read_ret == SYSTEM_ERROR) {
			throw std::runtime_error("read failed");
		}
		// todo: need?
		// buffers_.Delete(client_fd);
		// event_monitor_.Delete(client_fd);
		return;
	}
}

void Server::RunHttp(const event::Event &event) {
	const int client_fd = event.fd;

	// Prepare to http.Run()
	const DtoClientInfos &client_infos = GetClientInfos(client_fd);
	const DtoServerInfos &server_infos = GetServerInfos(client_fd);
	DebugDto(client_infos, server_infos);

	http::HttpResult http_result = mock_http.Run(client_infos, server_infos);
	// Check if it's ready to start write/send.
	// If not completed, the request will be re-read by the event_monitor.
	if (!http_result.is_response_complete) {
		return;
	}
	utils::Debug("server", "received all request from client", client_fd);
	std::cerr << buffers_.GetRequest(client_fd) << std::endl;
	buffers_.AddResponse(client_fd, http_result.response);
	event_monitor_.Update(event, event::EVENT_WRITE);
}

void Server::SendResponse(int client_fd) {
	const std::string &response = buffers_.GetResponse(client_fd);

	send(client_fd, response.c_str(), response.size(), 0);
	utils::Debug("server", "send response to client", client_fd);

	// todo: connection keep-aliveならdisconnectしない
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
		context_.GetVirtualServerList();

	typedef VirtualServerStorage::VirtualServerList::const_iterator ItVirtualServer;
	for (ItVirtualServer it = all_virtual_server_list.begin(); it != all_virtual_server_list.end();
		 ++it) {
		const VirtualServer           &virtual_server = *it;
		const VirtualServer::PortList &ports          = virtual_server.GetPorts();

		// 各virtual serverの全portをsocket通信
		typedef VirtualServer::PortList::const_iterator ItPort;
		for (ItPort it_port = ports.begin(); it_port != ports.end(); ++it_port) {
			// create ServerInfo & listen
			ServerInfo server_info(*it_port);
			const int  server_fd = connection_.Connect(server_info);
			server_info.SetSockFd(server_fd);

			// add to context
			context_.AddServerInfo(server_info, &virtual_server);
			event_monitor_.Add(server_fd, event::EVENT_READ);
			utils::Debug("server", "listen", server_fd);
		}
	}
}

} // namespace server
