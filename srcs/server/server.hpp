#ifndef SERVER_SERVER_HPP_
#define SERVER_SERVER_HPP_

#include "config_parse/context.hpp"
#include "connection.hpp"
#include "context_manager.hpp"
#include "epoll.hpp"
#include "http_result.hpp"
#include "message_manager.hpp"
#include "mock_http.hpp"
#include <list>
#include <string>

namespace server {

struct DtoClientInfos;
struct DtoServerInfos;

class Server {
  public:
	typedef std::list<config::context::ServerCon> ConfigServers;
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
	void       AddVirtualServers(const ConfigServers &config_servers);
	ServerInfo Listen(const std::string &host, unsigned int port);
	void       HandleEvent(const event::Event &event);
	void       HandleNewConnection(int server_fd);
	void       HandleExistingConnection(const event::Event &event);
	void       ReadRequest(int client_fd);
	void       RunHttp(const event::Event &event);
	void       SendResponse(int client_fd);
	void       HandleTimeoutMessages();
	void       KeepConnection(int client_fd);
	void       Disconnect(int client_fd);
	void       UpdateEventInResponseComplete(
			  const message::ConnectionState connection_state, const event::Event &event
		  );
	void UpdateConnectionAfterSendResponse(
		int client_fd, const message::ConnectionState connection_state
	);
	void SetNonBlockingMode(int sock_fd);
	// for Server to Http
	DtoClientInfos        GetClientInfos(int client_fd) const;
	VirtualServerAddrList GetVirtualServerList(int client_fd) const;

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
	http::MockHttp mock_http_;
	// message manager with time control
	MessageManager message_manager_;
};

} // namespace server

#endif /* SERVER_SERVER_HPP_ */
