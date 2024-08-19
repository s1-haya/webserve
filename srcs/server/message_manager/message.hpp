#ifndef SERVER_MESSAGE_HPP_
#define SERVER_MESSAGE_HPP_

#include <ctime>

namespace server {

class Message {
  public:
	typedef std::time_t Time;

	Message(int client_fd);
	~Message();
	Message(const Message &other);
	Message &operator=(const Message &other);
	// function
	bool IsTimeoutExceeded(double timeout_sec) const;

  private:
	Message();
	// functions
	static Time GetCurrentTime();
	// variables
	int  client_fd_;
	Time start_time_;
};

} // namespace server

#endif /* SERVER_MESSAGE_HPP_ */
