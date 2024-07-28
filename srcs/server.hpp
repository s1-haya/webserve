#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "_config.hpp"
#include "buffer.hpp"
#include "connection.hpp"
#include "epoll.hpp"
#include "sock_context.hpp"
#include "virtual_server_storage.hpp"
#include <list>
#include <string>

namespace server {

// todo: tmp
struct TempConfig {
	std::string            server_name;
	std::list<std::string> locations;
	std::list<std::string> ports;
};

class Server {
  public:
	typedef std::list<TempConfig> TempAllConfig; // todo: tmp
	explicit Server(const _config::Config::ConfigData &config);
	~Server();
	void Run();

  private:
	Server();
	// prohibit copy
	Server(const Server &other);
	Server &operator=(const Server &other);
	// functions
	void        AddVirtualServers(const TempAllConfig &all_configs); // todo: tmp
	void        Init();
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
