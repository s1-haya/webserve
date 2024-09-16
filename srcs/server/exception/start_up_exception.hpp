#ifndef SERVER_EXCEPTION_START_UP_EXCEPTION_HPP_
#define SERVER_EXCEPTION_START_UP_EXCEPTION_HPP_

#include <stdexcept>
#include <string>

namespace server {

class StartUpException : public std::runtime_error {
  public:
	explicit StartUpException(const std::string &message);
};

} // namespace server

#endif /* SERVER_EXCEPTION_START_UP_EXCEPTION_HPP_ */
