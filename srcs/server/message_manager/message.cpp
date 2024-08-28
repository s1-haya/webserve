#include "message.hpp"

namespace server {
namespace message {

Message::Message(int client_fd)
	: client_fd_(client_fd),
	  start_time_(GetCurrentTime()),
	  is_timeout_(false),
	  connection_state_(KEEP) {}

Message::Message(int client_fd, const std::string &request_buf)
	: client_fd_(client_fd),
	  start_time_(GetCurrentTime()),
	  is_timeout_(false),
	  connection_state_(KEEP),
	  request_buf_(request_buf) {}

Message::~Message() {}

Message::Message(const Message &other) {
	*this = other;
}

Message &Message::operator=(const Message &other) {
	if (this != &other) {
		client_fd_   = other.client_fd_;
		start_time_  = other.start_time_;
		is_timeout_  = other.is_timeout_;
		request_buf_ = other.request_buf_;
		response_    = other.response_;
	}
	return *this;
}

bool Message::IsNewTimeoutExceeded(double timeout_sec) const {
	if (is_timeout_) {
		return false;
	}
	const Time   current_time  = GetCurrentTime();
	const double diff_time_sec = std::difftime(current_time, start_time_);
	return diff_time_sec >= timeout_sec;
}

int Message::GetFd() const {
	return client_fd_;
}

ConnectionState Message::GetConnectionState() const {
	return connection_state_;
}

const std::string &Message::GetRequestBuf() const {
	return request_buf_;
}

const std::string &Message::GetResponse() const {
	return response_;
}

void Message::AddRequestBuf(const std::string &request_buf) {
	request_buf_ += request_buf;
}

void Message::SetTimeout() {
	is_timeout_ = true;
}

void Message::SetNewRequestBuf(const std::string &request_buf) {
	request_buf_ = request_buf;
}

void Message::SetResponse(ConnectionState connection_state, const std::string &response) {
	connection_state_ = connection_state;
	response_         = response;
}

Message::Time Message::GetCurrentTime() {
	return std::time(NULL);
}

} // namespace message
} // namespace server
