#ifndef SERVER_SEND_HPP_
#define SERVER_SEND_HPP_

#include "utils.hpp"
#include <string>

namespace server {

/**
 * @brief Sends a string to the client and returns the result.
 *
 * Sends the given string to the client using the provided file descriptor.
 * The result is `false` if an error occurs, otherwise `true`.
 * If any part of the string is not sent, it is stored in the result value.
 */
class Send {
  public:
	typedef utils::Result<std::string> SendResult;

	// function
	static SendResult SendStr(int client_fd, const std::string &send_str);

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
