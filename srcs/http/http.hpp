#ifndef HTTP_HPP_
#define HTTP_HPP_

#include "debug.hpp" // todo: tmp
#include <map>
#include <string>

namespace http {

class Http {
  public:
	enum MessageType {
		HTTP_METHOD,
		HTTP_REQUEST_TARGET,
		HTTP_VERSION,
		HTTP_CONTENT,
		HTTP_STATUS,
		HTTP_STATUS_TEXT
	};
	typedef std::map<MessageType, std::string> RequestMessage;
	explicit Http(const std::string &read_buf);
	~Http();
	std::string CreateResponse();

  private:
	Http();
	// prohibit copy
	Http(const Http &other);
	Http &operator=(const Http &other);
	void  ParseRequest(const std::string &read_buf);
	void  ReadPathContent();
	// variable
	RequestMessage request_;
};

} // namespace http

#endif /* HTTP_HPP_ */
