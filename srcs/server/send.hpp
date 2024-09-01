#ifndef SERVER_SEND_HPP_
#define SERVER_SEND_HPP_

#include "utils.hpp"
#include <string>
#include <sys/types.h> // ssize_t

namespace server {

/**
 * @brief Sends an response to the client and returns the result.
 *
 * Sends the given response to the client using the provided file descriptor.
 * The result is `false` if an error occurs, otherwise `true`.
 * If any part of the response is not sent, it is stored in the result value.
 */
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
