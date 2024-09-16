#ifndef TEST_CLIENT_HPP_
#define TEST_CLIENT_HPP_

#include <netinet/in.h> // struct sockaddr_in
#include <string>

class Client {
  public:
	explicit Client(unsigned int port);
	~Client();
	void SendRequestAndReceiveResponse(const std::string &message);

  private:
	Client();
	// prohibit copy
	Client(const Client &other);
	Client &operator=(const Client &other);
	void    Init();
	// const
	static const int          SYSTEM_ERROR = -1;
	static const unsigned int BUFFER_SIZE  = 1024;
	// socket
	unsigned int       port_;
	int                sock_fd_;
	struct sockaddr_in sock_addr_;
};

#endif /* TEST_CLIENT_HPP_ */
