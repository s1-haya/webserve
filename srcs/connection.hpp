#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include <netinet/in.h> // struct sockaddr_in
#include <string>

namespace server {

class SockInfo;

class Connection {
  public:
	static int Init(SockInfo &server_sock_info);
	static int Accept(SockInfo &sock_info);

  private:
	Connection();
	~Connection();
	// prohibit copy
	Connection(const Connection &other);
	Connection &operator=(const Connection &other);
	// const
	static const int SYSTEM_ERROR = -1;
};

} // namespace server

#endif /* CONNECTION_HPP_ */
