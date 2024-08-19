#include "message_manager.hpp"

namespace server {

// todo: tmp
const double MessageManager::TIMEOUT = 3.0;

MessageManager::MessageManager() {}

MessageManager::~MessageManager() {}

MessageManager::MessageManager(const MessageManager &other) {
	*this = other;
}

MessageManager &MessageManager::operator=(const MessageManager &other) {
	if (this != &other) {
		messages_ = other.messages_;
	}
	return *this;
}

void MessageManager::AddNewMessage(int client_fd) {
	message::Message message(client_fd);
	messages_.push_back(message);
}

// before: MessageList{3,4,5}
// (if timeout fd 3,4)
// return: TimeoutFds{3,4}
// after : MessageList{5}
MessageManager::TimeoutFds MessageManager::GetTimeoutFds() {
	TimeoutFds timeout_fds_;

	typedef MessageList::const_iterator Itr;
	Itr                                 it = messages_.begin();
	while (it != messages_.end()) {
		const Itr next = ++Itr(it);

		const message::Message &message = *it;
		if (!message.IsTimeoutExceeded(TIMEOUT)) {
			break;
		}
		timeout_fds_.push_back(message.GetFd());
		messages_.pop_front();

		it = next;
	}
	return timeout_fds_;
}

} // namespace server
