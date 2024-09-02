#include "http_exception.hpp"

namespace http {

HttpException::HttpException(const std::string &error_message, StatusCode status_code)
	: runtime_error(error_message), status_code_(status_code) {}

StatusCode HttpException::GetStatusCode() const {
	return status_code_;
}

} // namespace http
