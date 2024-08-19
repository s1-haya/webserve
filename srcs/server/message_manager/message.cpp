#include "message.hpp"

namespace server {

Message::Message(int client_fd) : client_fd_(client_fd), start_time_(GetCurrentTime()) {}

Message::~Message() {}

Message::Message(const Message &other) {
	*this = other;
}

Message &Message::operator=(const Message &other) {
	if (this != &other) {
		client_fd_  = other.client_fd_;
		start_time_ = other.start_time_;
	}
	return *this;
}

bool Message::IsTimeoutExceeded(double timeout_sec) const {
	const Time   current_time  = GetCurrentTime();
	const double diff_time_sec = std::difftime(current_time, start_time_);
	return diff_time_sec >= timeout_sec;
}

Message::Time Message::GetCurrentTime() {
	return std::time(NULL);
}

} // namespace server
