#ifndef SERVER_MESSAGE_HPP_
#define SERVER_MESSAGE_HPP_

#include <ctime>
#include <string>

namespace server {
namespace message {

class Message {
  public:
	typedef std::time_t Time;

	explicit Message(int client_fd);
	Message(int client_fd, const std::string &request_buf);
	~Message();
	Message(const Message &other);
	Message &operator=(const Message &other);
	// function
	bool IsTimeoutExceeded(double timeout_sec) const;
	void AddRequestBuf(const std::string &request_buf);
	// getter
	int                GetFd() const;
	bool               GetIsConnectionKeep() const;
	const std::string &GetRequestBuf() const;
	const std::string &GetResponse() const;
	// setter
	void SetNewRequestBuf(const std::string &request_buf);
	void SetResponse(bool is_connection_keep, const std::string &response);

  private:
	Message();
	// functions
	static Time GetCurrentTime();
	// variables
	int         client_fd_;
	Time        start_time_;
	bool        is_connection_keep_;
	std::string request_buf_;
	std::string response_;
};

} // namespace message
} // namespace server

#endif /* SERVER_MESSAGE_HPP_ */
