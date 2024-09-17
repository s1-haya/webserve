#include "start_up_exception.hpp"

namespace server {

StartUpException::StartUpException(const std::string &message) : std::runtime_error(message) {}

} // namespace server
