#ifndef HTTP_EXCEPTION_HPP_
#define HTTP_EXCEPTION_HPP_

#include "status_code.hpp"
#include <stdexcept>
#include <string>

namespace http {

class HttpException : public std::runtime_error {
  public:
	HttpException(const std::string &error_message, const StatusCode &status_code);
	~HttpException() throw();
	const StatusCode &GetStatusCode() const throw();

  private:
	StatusCode status_code_;
};

} // namespace http

#endif
