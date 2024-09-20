#ifndef UTILS_SYSTEM_EXCEPTION_HPP_
#define UTILS_SYSTEM_EXCEPTION_HPP_

#include <stdexcept>
#include <string>

namespace utils {

class SystemException : public std::runtime_error {
  public:
	explicit SystemException(int error_number);
	SystemException(const std::string &message, int error_number);
	int GetErrorNumber() const;

  private:
	int error_number_;
};

} // namespace utils

#endif
