#ifndef HTTP_RESPONSE_EXCEPTION_HPP_
#define HTTP_RESPONSE_EXCEPTION_HPP_

#include "status_code.hpp"
#include <stdexcept>
#include <string>

namespace http {

class HttpException : public std::runtime_error {
  public:
	explicit HttpException(const std::string &error_message, EStatusCode status_code);
	EStatusCode GetStatusCode() const;

  private:
	EStatusCode status_code_;
};

} // namespace http

#endif
