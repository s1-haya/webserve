#include "http_exception.hpp"

namespace http {

HttpException::HttpException(const std::string &error_message, const StatusCode &status_code)
	: runtime_error(error_message), status_code_(status_code) {}

HttpException::~HttpException() throw() {}

const StatusCode &HttpException::GetStatusCode() const throw() {
	return status_code_;
}

} // namespace http
