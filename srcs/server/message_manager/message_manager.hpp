#ifndef SERVER_MESSAGE_MANAGER_HPP_
#define SERVER_MESSAGE_MANAGER_HPP_

namespace server {

class MessageManager {
  public:
	MessageManager();
	~MessageManager();
	MessageManager(const MessageManager &other);
	MessageManager &operator=(const MessageManager &other);
};

} // namespace server

#endif /* SERVER_MESSAGE_MANAGER_HPP_ */
