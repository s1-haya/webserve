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

  private:
	// variable
	MessageMap messages_;
};

} // namespace server

#endif /* SERVER_MESSAGE_MANAGER_HPP_ */
