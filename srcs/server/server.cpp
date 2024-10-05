#include "server.hpp"
#include "client_info.hpp"
#include "define.hpp"
#include "event.hpp"
#include "send.hpp"
#include "start_up_exception.hpp"
#include "system_exception.hpp"
#include "utils.hpp"
#include "virtual_server.hpp"
#include <cerrno>
#include <cstring>      // strerror
#include <fcntl.h>      // fcntl
#include <sys/socket.h> // socket
#include <unistd.h>     // close

namespace server {

// todo: tmp
const double Server::REQUEST_TIMEOUT = 3.0;

namespace {

typedef std::set<Server::HostPortPair> HostPortSet;

typedef Server::VirtualServerList::const_iterator   ItVirtualServer;
typedef VirtualServer::HostPortList::const_iterator ItHostPort;

VirtualServer::LocationList ConvertLocations(const config::context::LocationList &config_locations
) {
	VirtualServer::LocationList location_list;

	typedef config::context::LocationList::const_iterator Itr;
	for (Itr it = config_locations.begin(); it != config_locations.end(); ++it) {
		Location location;
		location.request_uri      = it->request_uri;
		location.alias            = it->alias;
		location.index            = it->index;
		location.autoindex        = it->autoindex;
		location.allowed_methods  = it->allowed_methods;
		location.redirect         = it->redirect;
		location.cgi_extension    = it->cgi_extension;
		location.upload_directory = it->upload_directory;

		location_list.push_back(location);
	}
	return location_list;
}

void AddResolvedHostPort(
	HostPortSet &host_port_set, const Connection::IpList &ip_list, unsigned int port
) {
	typedef Connection::IpList::const_iterator Itr;
	for (Itr it = ip_list.begin(); it != ip_list.end(); ++it) {
		const Server::HostPortPair host_port = std::make_pair(*it, port);

		typedef std::pair<HostPortSet::const_iterator, bool> InsertResult;
		const InsertResult result = host_port_set.insert(host_port);
		if (result.second == false) {
			// duplicate host:port
			throw std::invalid_argument("invalid host:port");
		}
	}
}

VirtualServer::HostPortList ConvertHostPortSetToList(const HostPortSet &host_ports_set) {
	return VirtualServer::HostPortList(host_ports_set.begin(), host_ports_set.end());
}

VirtualServer::HostPortList ConvertHostPorts(const config::context::HostPortList &config_host_ports
) {
	// Temporary std::set for checking duplicates of host:port for each virtual server.
	HostPortSet host_ports_set;

	typedef config::context::HostPortList::const_iterator Itr;
	for (Itr it = config_host_ports.begin(); it != config_host_ports.end(); ++it) {
		const Connection::IpList ip_list = Connection::ResolveHostName(it->first);
		AddResolvedHostPort(host_ports_set, ip_list, it->second);
	}
	return ConvertHostPortSetToList(host_ports_set);
}

VirtualServer ConvertToVirtualServer(const config::context::ServerCon &config_server) {
	return VirtualServer(
		config_server.server_names,
		ConvertLocations(config_server.location_con),
		ConvertHostPorts(config_server.host_ports),
		config_server.client_max_body_size,
		config_server.error_page
	);
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
	try {
		AddVirtualServers(config_servers);
	} catch (const std::exception &e) {
		throw StartUpException(e.what());
	}
}

Server::~Server() {}

// throw SystemErrorException
void Server::Run() {
	utils::Debug("server", "run server");

	while (true) {
		const event::EventList events = event_monitor_.GetEventList();
		for (event::ItEventList it = events.begin(); it != events.end(); ++it) {
			const event::Event &event = *it;
			HandleEvent(event);
		}
		HandleTimeoutMessages();
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
	const AcceptResult result = Accept(server_fd);
	if (!result.IsOk()) {
		return;
	}

	const ClientInfo &new_client_info = result.GetValue();
	const int         client_fd       = new_client_info.GetFd();
	SetNonBlockingMode(client_fd);

	// add client_info, message, event
	context_.AddClientInfo(new_client_info);
	message_manager_.AddNewMessage(client_fd);
	AddEventRead(client_fd);
	utils::Debug(
		"server",
		"add new client IP: " + new_client_info.GetIp() + " / listen server: " +
			new_client_info.GetListenIp() + ":" + utils::ToString(new_client_info.GetListenPort()),
		client_fd
	);
}

void Server::HandleExistingConnection(const event::Event &event) {
	if (event.type & event::EVENT_ERROR) {
		HandleErrorEvent(event.fd);
		return;
	}
	if (event.type & event::EVENT_HANGUP) {
		HandleHangUpEvent(event);
		return;
	}
	if (event.type & event::EVENT_READ) {
		HandleReadEvent(event);
	}
	if (event.type & event::EVENT_WRITE) {
		HandleWriteEvent(event.fd);
	}
	// Call RunHttpAndCgi() if request_buf contains data, even without a read event.
	if (IsHttpRequestBufExist(event.fd)) {
		RunHttpAndCgi(event);
	}
}

bool Server::IsMessageExist(int fd) const {
	// client_fd
	if (message_manager_.IsMessageExist(fd)) {
		return true;
	}
	// pipe_fd
	if (cgi_manager_.IsCgiExist(fd)) {
		return true;
	}
	// disconnected client_fd/pipe_fd
	return false;
}

void Server::HandleErrorEvent(int fd) {
	if (!IsMessageExist(fd)) {
		return;
	}
	const int client_fd = IsCgi(fd) ? cgi_manager_.GetClientFd(fd) : fd;
	std::cout << "HandleErrorEvent" << std::endl;
	std::cout << "client_fd: " << client_fd << std::endl;
	SetInternalServerError(client_fd);
}

void Server::HandleHangUpEvent(const event::Event &event) {
	const int fd = event.fd;
	if (!IsMessageExist(fd)) {
		return;
	}
	if (IsCgi(fd)) {
		const int                     client_fd      = cgi_manager_.GetClientFd(fd);
		const CgiManager::GetFdResult read_fd_result = cgi_manager_.GetReadFd(client_fd);
		// except on read_fd
		if (!read_fd_result.IsOk() || read_fd_result.GetValue() != fd) {
			SetInternalServerError(client_fd);
			return;
		}
		// If it's read_fd, keep reading until read returns 0
		HandleReadEvent(event);
		return;
	}
	// fd == client_fd
	SetInternalServerError(fd);
}

void Server::HandleReadEvent(const event::Event &event) {
	const int fd = event.fd;
	// Prevent ReadStr() if Disconnect() was called during EVENT_WRITE handling.
	if (!IsMessageExist(fd)) {
		return;
	}

	const Read::ReadResult read_result = Read::ReadStr(fd);
	if (IsCgi(fd)) {
		HandleCgiReadResult(fd, read_result);
		return;
	}
	HandleHttpReadResult(event, read_result);
}

http::ClientInfos Server::GetClientInfos(int client_fd) const {
	http::ClientInfos client_infos;
	client_infos.fd                 = client_fd;
	client_infos.ip                 = context_.GetClientIp(client_fd);
	client_infos.listen_server_port = context_.GetListenServerPort(client_fd);
	client_infos.request_buf        = message_manager_.GetRequestBuf(client_fd);
	return client_infos;
}

VirtualServerAddrList Server::GetVirtualServerList(int client_fd) const {
	return context_.GetVirtualServerAddrList(client_fd);
}

void Server::HandleHttpReadResult(const event::Event &event, const Read::ReadResult &read_result) {
	const int client_fd = event.fd;

	if (!read_result.IsOk()) {
		SetInternalServerError(client_fd);
		return;
	}
	if (read_result.GetValue().read_size == 0) {
		// clientが正しくshutdownした場合・長さ0のデータグラムを受信した場合などにここに入るらしい
		return;
	}
	message_manager_.AddRequestBuf(client_fd, read_result.GetValue().read_buf);
	std::cerr << message_manager_.GetRequestBuf(client_fd) << std::endl;
}

bool Server::IsHttpRequestBufExist(int fd) const {
	if (IsCgi(fd)) {
		return false;
	}
	return !message_manager_.GetRequestBuf(fd).empty();
}

void Server::RunHttpAndCgi(const event::Event &event) {
	const int client_fd = event.fd;

	// Prepare to http.Run()
	const http::ClientInfos     &client_infos    = GetClientInfos(client_fd);
	const VirtualServerAddrList &virtual_servers = GetVirtualServerList(client_fd);

	http::HttpResult http_result = http_.Run(client_infos, virtual_servers);
	// Set the unused request_buf in Http.
	message_manager_.SetNewRequestBuf(client_fd, http_result.request_buf);
	// Check if it's ready to start write/send.
	// If not completed, the request will be re-read by the event_monitor.
	if (!http_result.is_response_complete) {
		message_manager_.SetIsCompleteRequest(client_fd, false);
		HandleCgi(client_fd, http_result.cgi_result);
		return;
	}
	message_manager_.SetIsCompleteRequest(client_fd, true);
	utils::Debug("server", "received all request from client", client_fd);

	const message::ConnectionState connection_state =
		http_result.is_connection_keep ? message::KEEP : message::CLOSE;
	message_manager_.AddNormalResponse(client_fd, connection_state, http_result.response);
	UpdateEventInResponseComplete(connection_state, event);
}

void Server::HandleWriteEvent(int fd) {
	// Prevent SendStr() if Disconnect() was called during EVENT_READ handling.
	if (!IsMessageExist(fd)) {
		return;
	}

	if (IsCgi(fd)) {
		SendCgiRequest(fd);
		return;
	}
	SendHttpResponse(fd);
}

void Server::SendHttpResponse(int client_fd) {
	message::Response              response         = message_manager_.PopHeadResponse(client_fd);
	const message::ConnectionState connection_state = response.connection_state;
	const std::string             &response_str     = response.response_str;

	const Send::SendResult send_result = Send::SendStr(client_fd, response_str);
	if (!send_result.IsOk()) {
		// Even if sending fails, continue the server
		// e.g., in case of a SIGPIPE(EPIPE) when the client disconnects
		utils::Debug("server", "failed to send response to client", client_fd);
		Disconnect(client_fd);
		return;
	}
	const std::string &new_response_str = send_result.GetValue();
	if (!new_response_str.empty()) {
		// If not everything was sent, re-add the remaining unsent part to the front
		message_manager_.AddPrimaryResponse(client_fd, connection_state, new_response_str);
		return;
	}
	utils::Debug("server", "send response to client", client_fd);

	if (!message_manager_.IsResponseExist(client_fd)) {
		ReplaceEvent(client_fd, event::EVENT_READ);
	}
	UpdateConnectionAfterSendResponse(client_fd, connection_state);
}

void Server::HandleTimeoutMessages() {
	// timeoutした全fdを取得
	const MessageManager::TimeoutFds &timeout_fds =
		message_manager_.GetNewTimeoutFds(REQUEST_TIMEOUT);

	// timeout用のresponseをセットしてevent監視をWRITEに変更
	typedef MessageManager::TimeoutFds::const_iterator Itr;
	for (Itr it = timeout_fds.begin(); it != timeout_fds.end(); ++it) {
		const int client_fd = *it;
		if (message_manager_.IsCompleteRequest(client_fd)) {
			Disconnect(client_fd);
			continue;
		}

		const http::HttpResult http_result = http_.GetErrorResponse(client_fd, http::TIMEOUT);
		message_manager_.AddPrimaryResponse(client_fd, message::CLOSE, http_result.response);
		ReplaceEvent(client_fd, event::EVENT_WRITE);
		utils::Debug("server", "timeout client", client_fd);
	}
}

// internal server error用のresponseをセットしてevent監視をWRITEに変更
void Server::SetInternalServerError(int client_fd) {
	if (cgi_manager_.IsCgiExist(client_fd)) {
		// Call Cgi's destructor -> close pipe_fd -> automatically deleted from epoll
		cgi_manager_.DeleteCgi(client_fd);
	}

	const http::HttpResult http_result = http_.GetErrorResponse(client_fd, http::INTERNAL_ERROR);
	message_manager_.AddPrimaryResponse(client_fd, message::CLOSE, http_result.response);
	ReplaceEvent(client_fd, event::EVENT_WRITE);
	utils::Debug("server", "internal server error to client", client_fd);
}

void Server::KeepConnection(int client_fd) {
	message_manager_.UpdateTime(client_fd);
	utils::Debug("server", "Connection: keep-alive client", client_fd);
}

// delete from event, message, context
void Server::Disconnect(int client_fd) {
	if (cgi_manager_.IsCgiExist(client_fd)) {
		// Call Cgi's destructor -> close pipe_fd -> automatically deleted from epoll
		cgi_manager_.DeleteCgi(client_fd);
	}
	// todo: client_save_dataがない場合に呼ばれても大丈夫な作りになってるか確認
	// HttpResult is not used.
	http_.GetErrorResponse(client_fd, http::INTERNAL_ERROR);
	event_monitor_.Delete(client_fd);
	message_manager_.DeleteMessage(client_fd);
	context_.DeleteClientInfo(client_fd);
	close(client_fd);
	utils::Debug("server", "Connection: close, disconnected client", client_fd);
	utils::Debug("------------------------------------------");
}

void Server::UpdateEventInResponseComplete(
	const message::ConnectionState connection_state, const event::Event &event
) {
	switch (connection_state) {
	case message::KEEP:
		AppendEventWrite(event);
		break;
	case message::CLOSE:
		ReplaceEvent(event.fd, event::EVENT_WRITE);
		break;
	default:
		break;
	}
}

void Server::UpdateConnectionAfterSendResponse(
	int client_fd, const message::ConnectionState connection_state
) {
	switch (connection_state) {
	case message::KEEP:
		KeepConnection(client_fd);
		break;
	case message::CLOSE:
		Disconnect(client_fd);
		break;
	default:
		break;
	}
}

void Server::AddEventRead(int sock_fd) {
	try {
		event_monitor_.Add(sock_fd, event::EVENT_READ);
	} catch (const SystemException &e) {
		utils::PrintError(e.what());
		SetInternalServerError(sock_fd);
	}
}

void Server::ReplaceEvent(int client_fd, uint32_t type) {
	try {
		event_monitor_.Replace(client_fd, type);
	} catch (const SystemException &e) {
		utils::PrintError(e.what());
		if (type & event::EVENT_WRITE) {
			Disconnect(client_fd);
		} else {
			SetInternalServerError(client_fd);
		}
	}
}

void Server::AppendEventWrite(const event::Event &event) {
	try {
		event_monitor_.Append(event, event::EVENT_WRITE);
	} catch (const SystemException &e) {
		utils::PrintError(e.what());
		Disconnect(event.fd);
	}
}

Server::AcceptResult Server::Accept(int server_fd) {
	AcceptResult result;
	try {
		ClientInfo new_client_info = Connection::Accept(server_fd);
		result.SetValue(new_client_info);
	} catch (const SystemException &e) {
		result.Set(false);
		utils::PrintError(e.what());
		SetInternalServerError(server_fd);
	}
	return result;
}

void Server::AddServerInfoToContext(const VirtualServerList &virtual_server_list) {
	for (ItVirtualServer it = virtual_server_list.begin(); it != virtual_server_list.end(); ++it) {
		const VirtualServer::HostPortList &host_ports = it->GetHostPortList();
		for (ItHostPort it_host_port = host_ports.begin(); it_host_port != host_ports.end();
			 ++it_host_port) {
			context_.AddServerInfo(*it_host_port);
		}
	}
}

// PortIpMap -> port1:{ip1, ip2}, port2:{ip1, 0.0.0.0}, ...
Server::PortIpMap Server::CreatePortIpMap(const VirtualServerList &virtual_server_list) {
	PortIpMap port_ip_map;
	for (ItVirtualServer it = virtual_server_list.begin(); it != virtual_server_list.end(); ++it) {
		const VirtualServer               &virtual_server = *it;
		const VirtualServer::HostPortList &host_ports     = virtual_server.GetHostPortList();
		for (ItHostPort it_host_port = host_ports.begin(); it_host_port != host_ports.end();
			 ++it_host_port) {
			const HostPortPair &host_port = *it_host_port;
			port_ip_map[host_port.second].insert(host_port.first);
			context_.AddMapping(host_port, &virtual_server);
		}
	}
	return port_ip_map;
}

void Server::Listen(const HostPortPair &host_port) {
	const int server_fd = connection_.Connect(host_port);
	SetNonBlockingMode(server_fd);

	context_.SetListenSockFd(host_port, server_fd);
	event_monitor_.Add(server_fd, event::EVENT_READ); // throw SystemException
	utils::Debug(
		"server", "listen " + host_port.first + ":" + utils::ToString(host_port.second), server_fd
	);
}

void Server::ListenAllHostPorts(const VirtualServerList &virtual_server_list) {
	const PortIpMap port_ip_map = CreatePortIpMap(virtual_server_list);

	typedef PortIpMap::const_iterator ItMap;
	for (ItMap it_map = port_ip_map.begin(); it_map != port_ip_map.end(); ++it_map) {
		const unsigned int port   = it_map->first;
		const IpSet       &ip_set = it_map->second;
		if (ip_set.find(IPV4_ADDR_ANY) != ip_set.end()) {
			Listen(std::make_pair(IPV4_ADDR_ANY, port));
			continue;
		}
		typedef IpSet::const_iterator ItSet;
		for (ItSet it_set = ip_set.begin(); it_set != ip_set.end(); ++it_set) {
			Listen(std::make_pair(*it_set, port));
		}
	}
}

void Server::Init() {
	const VirtualServerList &virtual_server_list = context_.GetAllVirtualServer();

	AddServerInfoToContext(virtual_server_list);
	try {
		ListenAllHostPorts(virtual_server_list);
	} catch (const std::exception &e) {
		throw StartUpException(e.what());
	}
}

void Server::SetNonBlockingMode(int sock_fd) {
	int flags = fcntl(sock_fd, F_GETFL);
	if (flags == SYSTEM_ERROR) {
		throw SystemException("fcntl F_GETFL failed: " + std::string(std::strerror(errno)));
	}
	flags |= O_NONBLOCK;
	if (fcntl(sock_fd, F_SETFL, flags) == SYSTEM_ERROR) {
		throw SystemException("fcntl F_SETFL failed: " + std::string(std::strerror(errno)));
	}
}

bool Server::IsCgi(int fd) const {
	return !message_manager_.IsMessageExist(fd);
}

void Server::HandleCgi(int client_fd, const http::CgiResult &cgi_result) {
	if (!cgi_result.is_cgi) {
		return;
	}
	try {
		// std::cout << client_fd << std::endl;
		cgi_manager_.AddNewCgi(client_fd, cgi_result.cgi_request);
		// RunCgi() is called only when a new Cgi is added via AddNewCgi().
		cgi_manager_.RunCgi(client_fd);
		AddEventForCgi(client_fd);
	} catch (const SystemException &e) {
		utils::PrintError(e.what());
		SetInternalServerError(client_fd);
	}
}

// throw(SystemException)
void Server::AddEventForCgi(int client_fd) {
	const CgiManager::GetFdResult read_fd_result = cgi_manager_.GetReadFd(client_fd);
	if (read_fd_result.IsOk()) {
		event_monitor_.Add(read_fd_result.GetValue(), event::EVENT_READ);
	}

	const CgiManager::GetFdResult write_fd_result = cgi_manager_.GetWriteFd(client_fd);
	if (write_fd_result.IsOk()) {
		event_monitor_.Add(write_fd_result.GetValue(), event::EVENT_WRITE);
	}
}

void Server::SendCgiRequest(int write_fd) {
	const int          client_fd   = cgi_manager_.GetClientFd(write_fd);
	const std::string &request_str = cgi_manager_.GetRequest(client_fd);

	const Send::SendResult send_result = Send::SendStr(write_fd, request_str);
	if (!send_result.IsOk()) {
		utils::Debug(
			"cgi", "Failed to send the request to the child process through pipe_fd", write_fd
		);
		Disconnect(client_fd);
		return;
	}
	const std::string &new_request_str = send_result.GetValue();
	cgi_manager_.ReplaceNewRequest(client_fd, new_request_str);
	if (new_request_str.empty()) {
		// Explicitly delete from epoll
		event_monitor_.Delete(write_fd);
	}
}

void Server::HandleCgiReadResult(int read_fd, const Read::ReadResult &read_result) {
	const int client_fd = cgi_manager_.GetClientFd(read_fd);

	if (!read_result.IsOk()) {
		utils::Debug(
			"cgi", "Failed to read the response from the child process through pipe_fd", read_fd
		);
		SetInternalServerError(client_fd);
		return;
	}
	const CgiResponseResult cgi_response_result =
		AddAndGetCgiResponse(client_fd, read_result.GetValue().read_buf);
	if (!cgi_response_result.IsOk()) {
		return;
	}
	utils::Debug("cgi", "Read the entire response from the child process through pipe_fd", read_fd);
	// Explicitly delete from cgi_manager
	cgi_manager_.DeleteCgi(client_fd);
	GetHttpResponseFromCgiResponse(client_fd, cgi_response_result.GetValue());
}

Server::CgiResponseResult Server::AddAndGetCgiResponse(int client_fd, const std::string &read_buf) {
	CgiResponseResult      cgi_response_result;
	const cgi::CgiResponse cgi_response = cgi_manager_.AddAndGetResponse(client_fd, read_buf);
	if (!cgi_response.is_response_complete) {
		cgi_response_result.Set(false);
		return cgi_response_result;
	}
	cgi_response_result.Set(true, cgi_response);
	return cgi_response_result;
}

void Server::GetHttpResponseFromCgiResponse(int client_fd, const cgi::CgiResponse &cgi_response) {
	http::HttpResult http_result = http_.GetResponseFromCgi(client_fd, cgi_response);

	message_manager_.SetNewRequestBuf(client_fd, http_result.request_buf);
	if (http_result.is_local_redirect) {
		std::cout << http_result.response << std::endl;
		message_manager_.AppendRequestBuf(client_fd, http_result.response);
		std::cout << "request_buf: " << message_manager_.GetRequestBuf(client_fd) << std::endl;
	} else if (!http_result.is_response_complete) {
		throw std::logic_error("GetResponseFromCgi: incorrect HttpResult");
	}
	message_manager_.SetIsCompleteRequest(client_fd, true);
	utils::Debug("server", "received all request from client", client_fd);

	const message::ConnectionState connection_state =
		http_result.is_connection_keep ? message::KEEP : message::CLOSE;
	UpdateEventInCgiResponseComplete(connection_state, client_fd);
	message_manager_.AddNormalResponse(client_fd, connection_state, http_result.response);
}

void Server::UpdateEventInCgiResponseComplete(
	const message::ConnectionState connection_state, int client_fd
) {
	switch (connection_state) {
	case message::KEEP:
		ReplaceEvent(client_fd, event::EVENT_READ | event::EVENT_WRITE);
		break;
	case message::CLOSE:
		ReplaceEvent(client_fd, event::EVENT_WRITE);
		break;
	default:
		break;
	}
}

} // namespace server
