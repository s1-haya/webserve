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

	// functions
	bool IsNewTimeoutExceeded(double timeout_sec) const;
	void UpdateTime();
	// request_buf
	void AddRequestBuf(const std::string &request_buf);
	void AppendRequestBuf(const std::string &request_buf);
	void DeleteRequestBuf();
	// response
	void     AddBackResponse(ConnectionState connection_state, const std::string &response_str);
	void     AddFrontResponse(ConnectionState connection_state, const std::string &response_str);
	Response PopFrontResponse();
	bool     IsResponseExist() const;

	// getter
	int                GetFd() const;
	bool               GetIsCompleteRequest() const;
	const std::string &GetRequestBuf() const;
	// setter
	void SetTimeout();
	void SetIsCompleteRequest(bool is_complete_request_message);

  private:
	Message();
	// function
	static Time GetCurrentTime();
	// variables
	int           client_fd_;
	Time          start_time_;
	bool          is_timeout_;
	bool          is_complete_request_message_;
	std::string   request_buf_;
	ResponseDeque responses_;
};

} // namespace message
} // namespace server

#endif /* SERVER_MESSAGE_HPP_ */
