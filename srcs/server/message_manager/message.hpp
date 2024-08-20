#ifndef SERVER_MESSAGE_HPP_
#define SERVER_MESSAGE_HPP_

#include <ctime>
#include <string>

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
	int                GetFd() const;
	const std::string &GetRequestBuf() const;
	const std::string &GetResponse() const;
	// setter
	void SetRequestBuf(const std::string &request_buf);
	void SetResponse(const std::string &response);

  private:
	Message();
	// functions
	static Time GetCurrentTime();
	// variables
	int  client_fd_;
	Time start_time_;
	// todo: add variables
	// bool is_connection_keep_;
	std::string request_buf_;
	std::string response_;
};

} // namespace message
} // namespace server

#endif /* SERVER_MESSAGE_HPP_ */
