#ifndef SERVER_INFO_HPP_
#define SERVER_INFO_HPP_

#include <string>

namespace server {

class ServerInfo {
  public:
	ServerInfo(int fd, const std::string &server_name, unsigned int port);
	~ServerInfo();

  private:
	ServerInfo();
	// prohibit copy
	ServerInfo(const ServerInfo &other);
	ServerInfo &operator=(const ServerInfo &other);
	// variables
	int          fd_;
	std::string  server_name_;
	unsigned int listen_port_;
};

} // namespace server

#endif /* SERVER_INFO_HPP_ */
