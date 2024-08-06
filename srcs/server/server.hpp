#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "buffer.hpp"
#include "config_parse/context.hpp"
#include "connection.hpp"
#include "epoll.hpp"
#include "sock_context.hpp"
#include "virtual_server_storage.hpp"
#include <list>
#include <string>

namespace server {

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
	void        AddVirtualServers(const ConfigServers &config_servers);
	void        HandleEvent(const event::Event &event);
	void        HandleNewConnection(int server_fd);
	void        HandleExistingConnection(const event::Event &event);
	void        ReadRequest(const event::Event &event);
	std::string CreateHttpResponse(int client_fd) const;
	void        SendResponse(int client_fd);
	// const
	static const int SYSTEM_ERROR = -1;
	// virtual servers storage
	VirtualServerStorage virtual_servers_;
	// connection
	Connection connection_;
	// context
	SockContext context_;
	// event poll
	epoll::Epoll event_monitor_;
	// request buffers
	Buffer buffers_;
};

} // namespace server

#endif /* SERVER_HPP_ */
