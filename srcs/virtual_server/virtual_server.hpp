#ifndef VIRTUAL_SERVER_HPP_
#define VIRTUAL_SERVER_HPP_

#include <string>

namespace server {

// virtual serverとして必要な情報を保持・取得する
class VirtualServer {
  public:
	// todo: configもらう？
	VirtualServer(const std::string &server_name, const std::string &location);
	~VirtualServer();
	VirtualServer(const VirtualServer &other);
	VirtualServer &operator=(const VirtualServer &other);

  private:
	// todo: add member(& operator=)
	std::string server_name_;
	std::string location_;
};

} // namespace server

#endif /* VIRTUAL_SERVER_HPP_ */
