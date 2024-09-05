#ifndef SERVER_CONTEXTMANAGER_SOCKCONTEXT_CLIENTINFO_HPP_
#define SERVER_CONTEXTMANAGER_SOCKCONTEXT_CLIENTINFO_HPP_

#include <string>

namespace server {

class ClientInfo {
  public:
	// default constructor: necessary for map's insert/[]
	ClientInfo();
	ClientInfo(int fd, const std::string &listen_host, unsigned int listen_port);
	~ClientInfo();
	ClientInfo(const ClientInfo &other);
	ClientInfo &operator=(const ClientInfo &other);
	// getter
	int                GetFd() const;
	const std::string &GetListenIp() const;
	unsigned int       GetListenPort() const;

  private:
	int          fd_;
	std::string  listen_ip_;
	unsigned int listen_port_;
};

} // namespace server

#endif /* SERVER_CONTEXTMANAGER_SOCKCONTEXT_CLIENTINFO_HPP_ */
