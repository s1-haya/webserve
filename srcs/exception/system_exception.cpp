#include "system_exception.hpp"

SystemException::SystemException(const std::string &message) : std::runtime_error(message) {}
