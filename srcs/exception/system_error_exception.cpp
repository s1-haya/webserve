#include "system_error_exception.hpp"

SystemErrorException::SystemErrorException(const std::string &message)
	: std::runtime_error(message) {}
