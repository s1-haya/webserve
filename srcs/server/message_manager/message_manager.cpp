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
	// todo: add logic_error
	messages_.insert(std::make_pair(client_fd, message));
}

// Remove one message that matches fd from the beginning of MessageList.
void MessageManager::DeleteMessage(int client_fd) {
	messages_.erase(client_fd);
}

// Delete all messages that have timed out, and return TimeoutFds list.
// ex)
//   before: MessageMap{3,4,5}
//   (if timeout fd 3,5)
//   return: TimeoutFds{3,5}
//   after : MessageMap{4}
MessageManager::TimeoutFds MessageManager::GetTimeoutFds(double timeout) {
	TimeoutFds timeout_fds_;

	typedef MessageMap::iterator Itr;
	for (Itr it = messages_.begin(); it != messages_.end();) {
		const message::Message &message = it->second;
		if (message.IsTimeoutExceeded(timeout)) {
			timeout_fds_.push_back(message.GetFd());
			const Itr it_erase = it;
			++it;
			messages_.erase(it_erase);
			continue;
		}
		++it;
	}
	return timeout_fds_;
}

// todo:
//   connection keep用。残request_bufなど保持するようになったら変更する。
//   まだServerからは呼ばれていない、unit testだけある
// Remove one message from the beginning and add a new message to the end.
void MessageManager::UpdateMessage(int client_fd) {
	// todo: connection keepができたらmessageのstart_time,request_buf更新
	// todo: 今は削除・新規追加(start_time更新)してるだけ
	DeleteMessage(client_fd);
	AddNewMessage(client_fd);
}

} // namespace server
