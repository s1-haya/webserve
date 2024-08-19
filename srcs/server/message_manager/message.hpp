#ifndef SERVER_MESSAGE_HPP_
#define SERVER_MESSAGE_HPP_

#include <ctime>

namespace server {
namespace message {

class Message {
  public:
	typedef std::time_t Time;

	Message(int client_fd);
	~Message();
	Message(const Message &other);
	Message &operator=(const Message &other);
	// function
	bool IsTimeoutExceeded(double timeout_sec) const;
	// getter
	int GetFd() const;

  private:
	Message();
	// functions
	static Time GetCurrentTime();
	// variables
	int  client_fd_;
	Time start_time_;
	// todo: add variables
	// bool is_connection_keep_;
	// std::string request_buf;
	// std::string response;
};

} // namespace message
} // namespace server

#endif /* SERVER_MESSAGE_HPP_ */
