#ifndef SERVER_MESSAGE_HPP_
#define SERVER_MESSAGE_HPP_

#include <ctime>
#include <deque>
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
	Response(ConnectionState connection_state, const std::string &response_str)
		: connection_state(connection_state), response_str(response_str) {};

	ConnectionState connection_state;
	std::string     response_str;
};

class Message {
  public:
	typedef std::time_t          Time;
	typedef std::deque<Response> ResponseDeque;

	explicit Message(int client_fd);
	Message(int client_fd, const std::string &request_buf);
	~Message();
	Message(const Message &other);
	Message &operator=(const Message &other);
	// function
	bool IsNewTimeoutExceeded(double timeout_sec) const;
	void UpdateTime();
	void AddRequestBuf(const std::string &request_buf);
	void DeleteRequestBuf();
	void DeleteOldestResponse();
	// getter
	int                GetFd() const;
	const std::string &GetRequestBuf() const;
	const Response    &GetResponse() const;
	// setter
	void SetTimeout();
	void SetNormalResponse(ConnectionState connection_state, const std::string &response_str);
	void SetPrimaryResponse(ConnectionState connection_state, const std::string &response_str);

  private:
	Message();
	// functions
	static Time GetCurrentTime();
	// variables
	int           client_fd_;
	Time          start_time_;
	bool          is_timeout_;
	std::string   request_buf_;
	ResponseDeque responses_;
};

} // namespace message
} // namespace server

#endif /* SERVER_MESSAGE_HPP_ */
