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

Message::Time Message::GetCurrentTime() {
	return std::time(NULL);
}

} // namespace server
