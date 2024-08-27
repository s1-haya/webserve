#ifndef SERVER_MESSAGE_HPP_
#define SERVER_MESSAGE_HPP_

#include <ctime>
#include <string>

namespace server {
namespace message {

enum ConnectionState {
	KEEP,
	CLOSE
};

// todo: tmp
struct Response {
	Response() : connection_state(KEEP) {};
	ConnectionState connection_state;
	std::string     response_str;
};

class Message {
  public:
	typedef std::time_t Time;

	explicit Message(int client_fd);
	Message(int client_fd, const std::string &request_buf);
	~Message();
	Message(const Message &other);
	Message &operator=(const Message &other);
	// function
	bool IsNewTimeoutExceeded(double timeout_sec) const;
	void AddRequestBuf(const std::string &request_buf);
	void DeleteRequestBuf();
	// getter
	int                GetFd() const;
	ConnectionState    GetConnectionState() const;
	const std::string &GetRequestBuf() const;
	const Response    &GetResponse() const;
	// setter
	void SetTimeout();
	void SetResponse(ConnectionState connection_state, const std::string &response_str);

  private:
	Message();
	// functions
	static Time GetCurrentTime();
	// variables
	int         client_fd_;
	Time        start_time_;
	bool        is_timeout_;
	std::string request_buf_;
	Response    response_;
};

} // namespace message
} // namespace server

#endif /* SERVER_MESSAGE_HPP_ */
