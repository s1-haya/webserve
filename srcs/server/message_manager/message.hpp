#ifndef SERVER_MESSAGE_HPP_
#define SERVER_MESSAGE_HPP_

namespace server {

class Message {
  public:
	Message();
	~Message();
	Message(const Message &other);
	Message &operator=(const Message &other);
};

} // namespace server

#endif /* SERVER_MESSAGE_HPP_ */
