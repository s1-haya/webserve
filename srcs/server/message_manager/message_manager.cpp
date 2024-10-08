#include "message_manager.hpp"
#include <stdexcept> // logic_error

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
	typedef std::pair<MessageMap::const_iterator, bool> InsertResult;

	message::Message message(client_fd);
	InsertResult     result = messages_.insert(std::make_pair(client_fd, message));
	if (result.second == false) {
		throw std::logic_error("AddNewMessage: message is already exist");
	}
}

// Remove one message that matches fd from the beginning of MessageList.
void MessageManager::DeleteMessage(int client_fd) {
	messages_.erase(client_fd);
}

bool MessageManager::IsMessageExist(int client_fd) const {
	return messages_.count(client_fd) != 0;
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

// todo: 全てのresponse_strをsend()できた場合かつkeep-aliveの場合に呼ばれる想定
// For Connection: keep-alive
void MessageManager::UpdateTime(int client_fd) {
	try {
		message::Message &message = messages_.at(client_fd);
		message.UpdateTime();
	} catch (const std::exception &e) {
		throw std::logic_error("UpdateTime: " + std::string(e.what()));
	}
}

void MessageManager::AddRequestBuf(int client_fd, const std::string &request_buf) {
	try {
		message::Message &message = messages_.at(client_fd);
		message.AddRequestBuf(request_buf);
	} catch (const std::exception &e) {
		throw std::logic_error("AddRequestBuf: " + std::string(e.what()));
	}
}

void MessageManager::AddFrontRequestBuf(int client_fd, const std::string &request_buf) {
	try {
		message::Message &message = messages_.at(client_fd);
		message.AddFrontRequestBuf(request_buf);
	} catch (const std::exception &e) {
		throw std::logic_error("AddFrontRequestBuf: " + std::string(e.what()));
	}
}

void MessageManager::SetNewRequestBuf(int client_fd, const std::string &request_buf) {
	try {
		message::Message &message = messages_.at(client_fd);
		message.DeleteRequestBuf();
		message.AddRequestBuf(request_buf);
	} catch (const std::exception &e) {
		throw std::logic_error("SetNewRequestBuf: " + std::string(e.what()));
	}
}

void MessageManager::AddNormalResponse(
	int client_fd, message::ConnectionState connection_state, const std::string &response
) {
	try {
		message::Message &message = messages_.at(client_fd);
		message.AddBackResponse(connection_state, response);
	} catch (const std::exception &e) {
		throw std::logic_error("AddNormalResponse: " + std::string(e.what()));
	}
}

void MessageManager::AddPrimaryResponse(
	int client_fd, message::ConnectionState connection_state, const std::string &response
) {
	try {
		message::Message &message = messages_.at(client_fd);
		message.AddFrontResponse(connection_state, response);
	} catch (const std::exception &e) {
		throw std::logic_error("AddPrimaryResponse: " + std::string(e.what()));
	}
}

message::Response MessageManager::PopHeadResponse(int client_fd) {
	try {
		message::Message &message = messages_.at(client_fd);
		return message.PopFrontResponse();
	} catch (const std::exception &e) {
		throw std::logic_error("PopHeadResponse: " + std::string(e.what()));
	}
}

bool MessageManager::IsResponseExist(int client_fd) const {
	try {
		const message::Message &message = messages_.at(client_fd);
		return message.IsResponseExist();
	} catch (const std::exception &e) {
		throw std::logic_error("IsResponseExist: " + std::string(e.what()));
	}
}

bool MessageManager::IsCompleteRequest(int client_fd) const {
	try {
		const message::Message &message = messages_.at(client_fd);
		return message.GetIsCompleteRequest();
	} catch (const std::exception &e) {
		throw std::logic_error("IsCompleteRequest: " + std::string(e.what()));
	}
}

const std::string &MessageManager::GetRequestBuf(int client_fd) const {
	try {
		const message::Message &message = messages_.at(client_fd);
		return message.GetRequestBuf();
	} catch (const std::exception &e) {
		throw std::logic_error("GetRequestBuf: " + std::string(e.what()));
	}
}

void MessageManager::SetIsCompleteRequest(int client_fd, bool is_complete_request) {
	try {
		message::Message &message = messages_.at(client_fd);
		message.SetIsCompleteRequest(is_complete_request);
	} catch (const std::exception &e) {
		throw std::logic_error("SetIsCompleteRequest: " + std::string(e.what()));
	}
}

} // namespace server
