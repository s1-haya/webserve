#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "_config.hpp"
#include "buffer.hpp"
#include "epoll.hpp"
#include <map>
#include <string>
#include <vector>

namespace server {

class Server {
  public:
	explicit Server(const _config::Config::ConfigData &config);
	~Server();
	void Run();
	// todo: tmp
	typedef std::pair<std::string, int>      TempConfig;
	typedef std::map<unsigned int, SockInfo> SockInfos;

  private:
	Server();
	// prohibit copy
	Server(const Server &other);
	Server &operator=(const Server &other);
	void    Init(const std::vector<TempConfig> &servers);
	void    HandleEvent(const event::Event &event);
	void    HandleNewConnection(const event::Event &event);
	void    HandleExistingConnection(const event::Event &event);
	void    ReadRequest(const event::Event &event);
	void    SendResponse(const event::Event &event);
	// const
	static const int SYSTEM_ERROR = -1;
	// SockInfo instances
	SockInfos sock_infos_;
	// event poll
	epoll::Epoll monitor_;
	// request buffers
	Buffer buffers_;
};

} // namespace server

#endif /* SERVER_HPP_ */
