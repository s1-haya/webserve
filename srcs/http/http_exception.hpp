#ifndef HTTP_EXCEPTION_HPP_
#define HTTP_EXCEPTION_HPP_

#include "status_code.hpp"
#include <stdexcept>
#include <string>

namespace http {

class HttpException : public std::runtime_error {
  public:
	explicit HttpException(const std::string &error_message, StatusCode status_code);
	StatusCode GetStatusCode() const;

  private:
	StatusCode status_code_;
};

} // namespace http

#endif
