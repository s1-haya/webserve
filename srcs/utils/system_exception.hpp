#ifndef SYSTEM_EXCEPTION_HPP_
#define SYSTEM_EXCEPTION_HPP_

#include <exception>
#include <string>

namespace utils {

class SystemException : public std::runtime_error {
  public:
	explicit SystemException(const std::string &message, int error_number)
		: std::runtime_error(message), error_number_(error_number) {}

	int GetErrorNumber() const {
		return error_number_;
	}

  private:
	int error_number_;
};

} // namespace utils

#endif
