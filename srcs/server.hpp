#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "buffer.hpp"
#include "config.hpp"
#include "debug.hpp" // todo: tmp
#include "epoll.hpp"
#include <netinet/in.h> // struct sockaddr_in
#include <string>

namespace server {

class Server {
  public:
	explicit Server(const Config::ConfigData &config);
	~Server();
	void Run();

  private:
	Server();
	// prohibit copy
	Server(const Server &other);
	Server &operator=(const Server &other);
	void    Init();
	void    HandleEvent(const Event &event);
	void    HandleNewConnection();
	void    HandleExistingConnection(const Event &event);
	void    ReadRequest(const Event &event);
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
	// event poll
	epoll::Epoll epoll_;
	// request buffers
	Buffer buffers_;
};

} // namespace server

#endif /* SERVER_HPP_ */
