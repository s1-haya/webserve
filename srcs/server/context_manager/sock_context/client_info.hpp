#ifndef SERVER_CONTEXTMANAGER_SOCKCONTEXT_CLIENTINFO_HPP_
#define SERVER_CONTEXTMANAGER_SOCKCONTEXT_CLIENTINFO_HPP_

#include <string>

namespace server {

class ClientInfo {
  public:
	// default constructor: necessary for map's insert/[]
	ClientInfo();
	explicit ClientInfo(int fd);
	~ClientInfo();
	ClientInfo(const ClientInfo &other);
	ClientInfo &operator=(const ClientInfo &other);
	// getter
	int GetFd() const;

  private:
	int fd_;
};

} // namespace server

#endif /* SERVER_CONTEXTMANAGER_SOCKCONTEXT_CLIENTINFO_HPP_ */
