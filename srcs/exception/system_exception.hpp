#ifndef SYSTEM_EXCEPTION_HPP_
#define SYSTEM_EXCEPTION_HPP_

#include <stdexcept>
#include <string>

class SystemException : public std::runtime_error {
  public:
	explicit SystemException(const std::string &message);
};

#endif
