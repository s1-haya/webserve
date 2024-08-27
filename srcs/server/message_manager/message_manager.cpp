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

void MessageManager::AddNewMessage(int client_fd, const std::string &request_buf) {
	message::Message message(client_fd, request_buf);
	// todo: add logic_error
	messages_.insert(std::make_pair(client_fd, message));
}

// Remove one message that matches fd from the beginning of MessageList.
void MessageManager::DeleteMessage(int client_fd) {
	messages_.erase(client_fd);
}

MessageManager::TimeoutFds MessageManager::GetNewTimeoutFds(double timeout) {
	TimeoutFds timeout_fds_;

	typedef MessageMap::iterator Itr;
	for (Itr it = messages_.begin(); it != messages_.end();) {
		message::Message &message = it->second;
		if (message.IsNewTimeoutExceeded(timeout)) {
			timeout_fds_.push_back(message.GetFd());
			message.SetTimeout();
		}
		++it;
	}
	return timeout_fds_;
}

// For Connection: keep-alive
// Copy only the read_buf from the old message, delete the old message, and add it as a new message.
void MessageManager::UpdateMessage(int client_fd) {
	const message::Message &old_message = messages_.at(client_fd);
	const std::string       request_buf = old_message.GetRequestBuf();
	DeleteMessage(client_fd);
	AddNewMessage(client_fd, request_buf);
}

const std::string &MessageManager::GetRequestBuf(int client_fd) const {
	const message::Message &message = messages_.at(client_fd);
	return message.GetRequestBuf();
}

const message::Response &MessageManager::GetResponse(int client_fd) const {
	const message::Message &message = messages_.at(client_fd);
	return message.GetResponse();
}

void MessageManager::AddRequestBuf(int client_fd, const std::string &request_buf) {
	message::Message &message = messages_.at(client_fd);
	message.AddRequestBuf(request_buf);
}

void MessageManager::SetNewRequestBuf(int client_fd, const std::string &request_buf) {
	message::Message &message = messages_.at(client_fd);
	message.DeleteRequestBuf();
	message.AddRequestBuf(request_buf);
}

void MessageManager::SetResponse(
	int client_fd, message::ConnectionState connection_state, const std::string &response
) {
	message::Message &message = messages_.at(client_fd);
	message.SetResponse(connection_state, response);
}

} // namespace server
