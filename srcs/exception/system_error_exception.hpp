#ifndef SYSTEM_ERROR_EXCEPTION_HPP_
#define SYSTEM_ERROR_EXCEPTION_HPP_

#include <stdexcept>
#include <string>

class SystemErrorException : public std::runtime_error {
  public:
	explicit SystemErrorException(const std::string &message);
};

#endif /* SYSTEM_ERROR_EXCEPTION_HPP_ */
