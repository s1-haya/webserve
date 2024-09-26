#include "system_exception.hpp"

SystemException::SystemException(const std::string &error_message)
	: std::runtime_error(error_message) {}
