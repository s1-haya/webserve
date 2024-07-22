#include "http_message.hpp"

namespace http {

const std::string SP                  = " ";
const std::string HTAB                = "\t";
const std::string OPTIONAL_WHITESPACE = SP + HTAB;
const std::string CRLF                = "\r\n";
const std::string HTTP_VERSION        = "HTTP/1.1";
const std::string CONNECTION          = "Connection";
const std::string CONTENT_LENGTH      = "Content-Length";

const std::string BASIC_METHODS[]       = {"GET", "DELETE", "POST"};
const std::size_t BASIC_METHODS_SIZE    = sizeof(BASIC_METHODS) / sizeof(BASIC_METHODS[0]);
const std::string BASIC_HEADER_FIELDS[] = {
	"Host",
	"User-Agent",
	"Accept",
	"Accept-Encoding",
	"Content-Type",
	"Content-Length",
	"Connection",
	"Authorization"
};
const std::size_t BASIC_HEADER_FIELDS_SIZE =
	sizeof(BASIC_HEADER_FIELDS) / sizeof(BASIC_HEADER_FIELDS[0]);

} // namespace http
