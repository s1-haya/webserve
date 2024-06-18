#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "buffer.hpp"
#include "config.hpp"
#include "debug.hpp" // todo: tmp
#include "epoll.hpp"
#include <netinet/in.h> // struct sockaddr_in
#include <string>

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
	void    AcceptNewConnection();
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
	Epoll epoll_;
	// request buffers
	Buffer buffers_;
};

#endif /* SERVER_HPP_ */
