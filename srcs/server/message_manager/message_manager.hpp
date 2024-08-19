#ifndef SERVER_MESSAGE_MANAGER_HPP_
#define SERVER_MESSAGE_MANAGER_HPP_

#include "message.hpp"
#include <list>

namespace server {

class MessageManager {
  public:
	// The order in which requests received
	typedef std::list<message::Message> MessageList;

	MessageManager();
	~MessageManager();
	MessageManager(const MessageManager &other);
	MessageManager &operator=(const MessageManager &other);

  private:
	// variable
	MessageList messages_;
};

} // namespace server

#endif /* SERVER_MESSAGE_MANAGER_HPP_ */
