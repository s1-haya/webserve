#ifndef SERVER_MESSAGE_MANAGER_HPP_
#define SERVER_MESSAGE_MANAGER_HPP_

#include "message.hpp"
#include <list>

namespace server {

class MessageManager {
  public:
	// The order in which requests received
	typedef std::list<message::Message> MessageList;
	typedef std::list<int>              TimeoutFds;

	MessageManager();
	~MessageManager();
	MessageManager(const MessageManager &other);
	MessageManager &operator=(const MessageManager &other);
	// functions
	void       AddNewMessage(int client_fd);
	TimeoutFds GetTimeoutFds();

  private:
	// const
	static const double TIMEOUT;
	// variable
	MessageList messages_;
};

} // namespace server

#endif /* SERVER_MESSAGE_MANAGER_HPP_ */
