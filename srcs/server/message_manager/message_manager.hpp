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
	void       DeleteSentResponseAndResetTime(int client_fd);
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

	// getter
	const std::string       &GetRequestBuf(int client_fd) const;
	const message::Response &GetResponse(int client_fd) const;

  private:
	// variable
	MessageMap messages_;
};

} // namespace server

#endif /* SERVER_MESSAGE_MANAGER_HPP_ */
