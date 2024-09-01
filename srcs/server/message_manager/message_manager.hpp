#ifndef SERVER_MESSAGE_MANAGER_HPP_
#define SERVER_MESSAGE_MANAGER_HPP_

#include "message.hpp"
#include <list>
#include <map>

namespace server {

class MessageManager {
  public:
	// Message for each fd
	typedef std::map<int, message::Message> MessageMap;
	typedef std::list<int>                  TimeoutFds;

	MessageManager();
	~MessageManager();
	MessageManager(const MessageManager &other);
	MessageManager &operator=(const MessageManager &other);

	// functions
	void       AddNewMessage(int client_fd);
	void       DeleteMessage(int client_fd);
	TimeoutFds GetNewTimeoutFds(double timeout);
	void       UpdateTime(int client_fd);
	// request_buf
	void AddRequestBuf(int client_fd, const std::string &request_buf);
	void SetNewRequestBuf(int client_fd, const std::string &request_buf);
	// response
	void AddNormalResponse(
		int client_fd, message::ConnectionState connection_state, const std::string &response
	);
	void AddPrimaryResponse(
		int client_fd, message::ConnectionState connection_state, const std::string &response
	);
	message::Response PopHeadResponse(int client_fd);
	bool              IsResponseExist(int client_fd) const;
	bool              IsCompleteRequest(int client_fd) const;

	// getter
	const std::string &GetRequestBuf(int client_fd) const;
	// setter
	void SetIsCompleteRequest(int client_fd, bool is_complete_request);

  private:
	// variable
	MessageMap messages_;
};

} // namespace server

#endif /* SERVER_MESSAGE_MANAGER_HPP_ */
