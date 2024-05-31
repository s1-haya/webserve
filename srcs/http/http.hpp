#ifndef HTTP_HPP_
#define HTTP_HPP_

#include "debug.hpp" // todo: tmp
#include <map>
#include <string>

class Http {
  public:
	enum MessageType {
		HTTP_METHOD,
		HTTP_PATH,
		HTTP_CONTENT,
		HTTP_STATUS
	};
	typedef std::map<MessageType, std::string> RequestMessage;
	// todo: tmp
	Http();
	~Http();

  private:
	// prohibit copy
	Http(const Http &other);
	Http &operator=(const Http &other);
	// variable
	RequestMessage request_;
};

#endif /* HTTP_HPP_ */
