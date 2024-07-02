#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include <string>

namespace server {

class Connection {
  public:
	static int Init(unsigned int port);
	static int Accept(int server_fd);

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
