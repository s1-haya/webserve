#include "message.hpp"
#include <stdexcept> // logic_error

namespace server {
namespace message {

Message::Message(int client_fd)
	: client_fd_(client_fd), start_time_(GetCurrentTime()), is_timeout_(false) {}

Message::Message(int client_fd, const std::string &request_buf)
	: client_fd_(client_fd),
	  start_time_(GetCurrentTime()),
	  is_timeout_(false),
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
		responses_   = other.responses_;
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

void Message::UpdateTime() {
	start_time_ = GetCurrentTime();
}

void Message::AddRequestBuf(const std::string &request_buf) {
	request_buf_ += request_buf;
}

void Message::DeleteRequestBuf() {
	request_buf_.clear();
}

void Message::AddBackResponse(ConnectionState connection_state, const std::string &response_str) {
	const Response response(connection_state, response_str);
	responses_.push_back(response);
}

void Message::AddFrontResponse(ConnectionState connection_state, const std::string &response_str) {
	const Response response(connection_state, response_str);
	responses_.push_front(response);
}

// deque.pop_front(): If there are no elements in the container, the behavior is undefined.
void Message::DeleteOldestResponse() {
	if (responses_.size() == 0) {
		throw std::logic_error("DeleteOldestResponse(): no response");
	}
	responses_.pop_front();
}

int Message::GetFd() const {
	return client_fd_;
}

const std::string &Message::GetRequestBuf() const {
	return request_buf_;
}

// deque.front(): Calling front on an empty container causes undefined behavior.
const Response &Message::GetResponse() const {
	if (responses_.size() == 0) {
		throw std::logic_error("GetResponse(): no response");
	}
	return responses_.front();
}

void Message::SetTimeout() {
	is_timeout_ = true;
}

Message::Time Message::GetCurrentTime() {
	return std::time(NULL);
}

} // namespace message
} // namespace server
