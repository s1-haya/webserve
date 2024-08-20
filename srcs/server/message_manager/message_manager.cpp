#include "message_manager.hpp"

namespace server {

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

// todo: map併用して高速化する？
// Remove one message that matches fd from the beginning of MessageList.
void MessageManager::DeleteMessage(int client_fd) {
	typedef MessageList::iterator Itr;
	for (Itr it = messages_.begin(); it != messages_.end(); ++it) {
		const message::Message &message = *it;
		if (message.GetFd() == client_fd) {
			messages_.erase(it);
			return;
		}
	}
}

// Look from the beginning of the MessageList,
// delete all messages that have timed out, and return TimeoutFds list.
// ex)
//   before: MessageList{3,4,5}
//   (if timeout fd 3,4)
//   return: TimeoutFds{3,4}
//   after : MessageList{5}
MessageManager::TimeoutFds MessageManager::GetTimeoutFds(double timeout) {
	TimeoutFds timeout_fds_;

	typedef MessageList::iterator Itr;
	Itr                           it = messages_.begin();
	while (it != messages_.end()) {
		const message::Message &message = *it;
		if (!message.IsTimeoutExceeded(timeout)) {
			break;
		}
		timeout_fds_.push_back(message.GetFd());
		it = messages_.erase(it);
	}
	return timeout_fds_;
}

// todo:
//   connection keep用。残request_bufなど保持するようになったら変更する。
//   まだServerからは呼ばれていない、unit testだけある
// Remove one message from the beginning and add a new message to the end.
void MessageManager::UpdateMessage(int client_fd) {
	typedef MessageList::iterator Itr;
	for (Itr it = messages_.begin(); it != messages_.end(); ++it) {
		const message::Message &message = *it;
		if (message.GetFd() == client_fd) {
			messages_.erase(it);
			AddNewMessage(client_fd);
			return;
		}
	}
}

} // namespace server
