#include "system_exception.hpp"
#include <cstring> // strerror

SystemException::SystemException(const std::string &error_message)
	: std::runtime_error(error_message) {}

SystemException::SystemException(int error_number)
	: std::runtime_error(std::strerror(error_number)) {}

SystemException::SystemException(const std::string &error_message, int error_number)
	: std::runtime_error(error_message), error_number_(error_number) {}

int SystemException::GetErrorNumber() const {
	return error_number_;
}
