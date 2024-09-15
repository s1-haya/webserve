#ifndef SYSTEM_EXCEPTION_HPP_
#define SYSTEM_EXCEPTION_HPP_

#include <stdexcept>
#include <string>

namespace utils {

class SystemException : public std::runtime_error {
  public:
	SystemException(const std::string &message, int error_number);
	int GetErrorNumber() const;

  private:
	int error_number_;
};

} // namespace utils

#endif
