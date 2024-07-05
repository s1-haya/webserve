#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include <netinet/in.h> // struct sockaddr_in
#include <string>

namespace server {

class Connection {
  public:
	static int Init(struct sockaddr_in &sock_addr, socklen_t addrlen);
	static int Accept(int server_fd, struct sockaddr_in &sock_addr, socklen_t *addrlen);

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
