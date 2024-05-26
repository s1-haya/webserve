#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "Config.hpp"
#include "debug.hpp"    // todo: tmp
#include <netinet/in.h> // struct sockaddr_in
#include <string>
#include <sys/epoll.h> // epoll

class Server {
  public:
	Server();
	Server(const Config::ConfigData &config);
	~Server();
	void Run();

  private:
	// prohibit copy
	Server(const Server &other);
	Server &operator=(const Server &other);
	void    Init();
	// const variables (todo: tmp)
	const std::string  server_name_;
	const unsigned int port_;
	// const
	static const int          kSystemErr  = -1;
	static const unsigned int kMaxEvents  = 1024;
	static const unsigned int kBufferSize = 1024;
	// socket
	struct sockaddr_in sock_addr_;
	socklen_t          addrlen_;
	int                server_fd_;
	// epoll
	int                epoll_fd_;
	struct epoll_event ev_;
};

#endif /* SERVER_HPP_ */
