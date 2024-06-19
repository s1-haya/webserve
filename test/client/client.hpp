#ifndef TEST_CLIENT_HPP_
#define TEST_CLIENT_HPP_

#include <netinet/in.h> // struct sockaddr_in
#include <string>

class Client {
  public:
	Client(
		unsigned int       port    = DEFAULT_PORT,
		const std::string &message = DEFAULT_MESSAGE
	);
	~Client();
	void UpdateMessage(const std::string &message);
	void SendRequestAndReceiveResponse();

  private:
	Client();
	// prohibit copy
	Client(const Client &other);
	Client &operator=(const Client &other);
	void    Init();
	// const
	static const int          SYSTEM_ERROR = -1;
	static const unsigned int DEFAULT_PORT = 8080;
	static const std::string  DEFAULT_MESSAGE;
	static const unsigned int BUFFER_SIZE = 1024;
	// socket
	unsigned int       port_;
	std::string        request_message_;
	int                sock_fd_;
	struct sockaddr_in sock_addr_;
};

#endif /* TEST_CLIENT_HPP_ */
