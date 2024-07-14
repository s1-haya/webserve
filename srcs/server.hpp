#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "_config.hpp"
#include "buffer.hpp"
#include "connection.hpp"
#include "epoll.hpp"
#include "sock_context.hpp"
#include <string>
#include <vector>

namespace server {

class Server {
  public:
	typedef std::pair<std::string, int> TempConfig; // todo: tmp
	typedef std::vector<SockInfo>       SockInfoVec;
	explicit Server(const _config::Config::ConfigData &config);
	~Server();
	void Run();

  private:
	Server();
	// prohibit copy
	Server(const Server &other);
	Server &operator=(const Server &other);
	void    Init(const SockInfoVec &sock_infos);
	void    HandleEvent(const event::Event &event);
	void    HandleNewConnection(int server_fd);
	void    HandleExistingConnection(const event::Event &event);
	void    ReadRequest(const event::Event &event);
	void    SendResponse(int client_fd);
	// const
	static const int SYSTEM_ERROR = -1;
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
