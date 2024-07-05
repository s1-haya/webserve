#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "_config.hpp"
#include "buffer.hpp"
#include "epoll.hpp"
#include <netinet/in.h> // struct sockaddr_in
#include <set>
#include <string>

namespace server {

class Server {
  public:
	typedef std::set<int> FdSet;
	explicit Server(const _config::Config::ConfigData &config);
	~Server();
	void Run();

  private:
	Server();
	// prohibit copy
	Server(const Server &other);
	Server &operator=(const Server &other);
	void    Init();
	void    HandleEvent(const event::Event &event);
	void    HandleNewConnection();
	void    HandleExistingConnection(const event::Event &event);
	void    ReadRequest(const event::Event &event);
	void    SendResponse(int client_fd);
	// const variables (todo: tmp)
	const std::string  server_name_;
	const unsigned int port_;
	// const
	static const int SYSTEM_ERROR = -1;
	// socket
	struct sockaddr_in sock_addr_;
	socklen_t          addrlen_;
	int                server_fd_;
	FdSet              listen_server_fds_;
	// event poll
	epoll::Epoll monitor_;
	// request buffers
	Buffer buffers_;
};

} // namespace server

#endif /* SERVER_HPP_ */
