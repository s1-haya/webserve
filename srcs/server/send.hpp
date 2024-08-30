#ifndef SERVER_SEND_HPP_
#define SERVER_SEND_HPP_

#include "utils.hpp"
#include <string>
#include <sys/types.h> // ssize_t

namespace server {

class Send {
  public:
	typedef std::string             Response;
	typedef utils::Result<Response> SendResult;

	// function
	static SendResult SendResponse(int client_fd, const Response &response);

  private:
	Send();
	~Send();
	// prohibit copy
	Send(const Send &other);
	Send &operator=(const Send &other);
	// const
	static const int SYSTEM_ERROR = -1;
};

} // namespace server

#endif /* SERVER_SEND_HPP_ */
