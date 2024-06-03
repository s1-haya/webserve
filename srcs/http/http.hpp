#ifndef HTTP_HPP_
#define HTTP_HPP_

#include "debug.hpp" // todo: tmp
#include <string>

class Http {
  public:
	enum MessageType {
		HTTP_METHOD,
		HTTP_PATH,
		HTTP_CONTENT,
		HTTP_STATUS,
		HTTP_STATUS_TEXT
	};
	typedef std::map<MessageType, std::string> RequestMessage;
	Http(const std::string &read_buf);
	~Http();
	const std::string CreateResponse();

  private:
	Http();
	// prohibit copy
	Http(const Http &other);
	Http &operator=(const Http &other);
};

#endif /* HTTP_HPP_ */
