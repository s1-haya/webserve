#ifndef SERVER_CONTEXTMANAGER_SOCKCONTEXT_SERVERINFO_HPP_
#define SERVER_CONTEXTMANAGER_SOCKCONTEXT_SERVERINFO_HPP_

#include <string>

namespace server {

class ServerInfo {
  public:
	typedef std::pair<std::string, unsigned int> HostPortPair;

	// default constructor: necessary for map's insert/[]
	ServerInfo();
	explicit ServerInfo(const HostPortPair &host_port);
	~ServerInfo();
	ServerInfo(const ServerInfo &other);
	ServerInfo &operator=(const ServerInfo &other);
	// getter
	int                GetFd() const;
	const std::string &GetHost() const;
	unsigned int       GetPort() const;
	// setter
	void SetSockFd(int fd);

  private:
	int          fd_;
	HostPortPair host_port_;
};

} // namespace server

#endif /* SERVER_CONTEXTMANAGER_SOCKCONTEXT_SERVERINFO_HPP_ */
