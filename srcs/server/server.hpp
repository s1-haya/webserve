#ifndef SERVER_SERVER_HPP_
#define SERVER_SERVER_HPP_

#include "cgi_manager.hpp"
#include "config_parse/context.hpp"
#include "connection.hpp"
#include "context_manager.hpp"
#include "epoll.hpp"
#include "http.hpp"
#include "http_result.hpp"
#include "message_manager.hpp"
#include "mock_http.hpp"
#include "read.hpp"
#include <list>
#include <string>

namespace server {

class Server {
  public:
	typedef std::list<config::context::ServerCon>   ConfigServers;
	typedef VirtualServer::HostPortPair             HostPortPair;
	typedef VirtualServerStorage::VirtualServerList VirtualServerList;
	typedef std::set<std::string>                   IpSet;
	typedef std::map<unsigned int, IpSet>           PortIpMap;
	typedef utils::Result<ClientInfo>               AcceptResult;

	explicit Server(const ConfigServers &config_servers);
	~Server();
	void Init();
	void Run();

  private:
	Server();
	// prohibit copy
	Server(const Server &other);
	Server &operator=(const Server &other);
	// functions
	void      AddVirtualServers(const ConfigServers &config_servers);
	void      AddServerInfoToContext(const VirtualServerList &virtual_server_list);
	void      ListenAllHostPorts(const VirtualServerList &virtual_server_list);
	PortIpMap CreatePortIpMap(const VirtualServerList &virtual_server_list);
	void      Listen(const HostPortPair &host_port);
	void      HandleEvent(const event::Event &event);
	void      HandleNewConnection(int server_fd);
	void      HandleExistingConnection(const event::Event &event);
	void      HandleReadEvent(const event::Event &event);
	void      HandleHttpReadResult(const event::Event &event, const Read::ReadResult &read_result);
	bool      IsHttpRequestBufExist(int fd) const;
	void      RunHttpAndCgi(const event::Event &event);
	void      HandleWriteEvent(int fd);
	void      SendHttpResponse(int client_fd);
	void      HandleTimeoutMessages();
	void      SetInternalServerError(int client_fd);
	void      KeepConnection(int client_fd);
	void      Disconnect(int client_fd);
	void      UpdateEventInResponseComplete(
			 const message::ConnectionState connection_state, const event::Event &event
		 );
	void UpdateConnectionAfterSendResponse(
		int client_fd, const message::ConnectionState connection_state
	);
	void SetNonBlockingMode(int sock_fd);
	// wrapper for epoll
	void AddEventRead(int sock_fd);
	void ReplaceEvent(int client_fd, event::Type type);
	void AppendEventWrite(const event::Event &event);
	// wrapper for connection
	AcceptResult Accept(int server_fd);
	// for Server to Http
	http::ClientInfos     GetClientInfos(int client_fd) const;
	VirtualServerAddrList GetVirtualServerList(int client_fd) const;
	// for Cgi
	bool IsCgi(int fd) const;
	void HandleCgi(int client_fd, const http::CgiResult &cgi_result);
	void AddEventForCgi(int client_fd);
	void HandleCgiReadResult(int pipe_fd, const Read::ReadResult &read_result);
	void SetCgiResponseToHttp(int pipe_fd, const std::string &read_buf);
	void SendCgiRequest(int pipe_fd);

	// const
	static const int    SYSTEM_ERROR = -1;
	static const double REQUEST_TIMEOUT;
	// context(virtual server,client)
	ContextManager context_;
	// connection
	Connection connection_;
	// event poll
	epoll::Epoll event_monitor_;
	// http
	http::Http http_;
	// message manager with time control
	MessageManager message_manager_;
	// cgi
	CgiManager cgi_manager_;
};

} // namespace server

#endif /* SERVER_SERVER_HPP_ */
