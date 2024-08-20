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
	TimeoutFds GetTimeoutFds(double timeout);
	void       UpdateMessage(int client_fd);
	void       AddRequestBuf(int client_fd, const std::string &request_buf);
	// getter
	const std::string &GetRequestBuf(int client_fd) const;
	const std::string &GetResponse(int client_fd) const;
	// setter
	void SetNewRequestBuf(int client_fd, const std::string &request_buf);
	void SetResponse(int client_fd, const std::string &response);

  private:
	// variable
	MessageMap messages_;
};

} // namespace server

#endif /* SERVER_MESSAGE_MANAGER_HPP_ */
