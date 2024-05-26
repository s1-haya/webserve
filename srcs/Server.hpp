#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "Config.hpp"
#include "debug.hpp" // todo: tmp
#include <string>

class Server {
  public:
	Server();
	Server(Config::MapConfig &config);
	~Server();
	void Run();

  private:
	// prohibit copy
	Server(const Server &other);
	Server &operator=(const Server &other);
	// const variables
	const std::string server_name_;
	const std::string port_;
	// todo: more variables(struct?)
};

#endif /* SERVER_HPP_ */
