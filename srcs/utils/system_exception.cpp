#include "system_exception.hpp"

namespace utils {

SystemException::SystemException(const std::string &error_message, int error_number)
	: runtime_error(error_message), error_number_(error_number) {}

int SystemException::GetErrorNumber() const {
	return error_number_;
}

} // namespace utils
