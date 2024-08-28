#ifndef STATUS_CODE_HPP_
#define STATUS_CODE_HPP_

#include <map>
#include <string>

namespace http {

enum StatusCode {
	OK                    = 200,
	CREATED               = 201,
	NO_CONTENT            = 204,
	MOVED_PERMANENTLY     = 301,
	BAD_REQUEST           = 400,
	FORBIDDEN             = 403,
	NOT_FOUND             = 404,
	INTERNAL_SERVER_ERROR = 500,
	NOT_IMPLEMENTED       = 501
};
typedef std::map<StatusCode, std::string> ReasonPhrase;
extern const ReasonPhrase                 reason_phrase;

} // namespace http

#endif
