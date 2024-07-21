#ifndef HTTP_MESSAGE_HPP_
#define HTTP_MESSAGE_HPP_

#include <string>

namespace http {

extern const std::string SP;
extern const std::string HTAB;
extern const std::string OPTIONAL_WHITESPACE;
extern const std::string CRLF;
extern const std::string HTTP_VERSION;
extern const std::string CONNECTION;
extern const std::string CONTENT_LENGTH;

extern const std::string BASIC_METHODS[];
extern const std::size_t BASIC_METHODS_SIZE;
extern const std::string BASIC_HEADER_FIELDS[];
extern const std::size_t BASIC_HEADER_FIELDS_SIZE;

} // namespace http

#endif /* HTTP_MESSAGE_HPP_ */
