#include "http_message.hpp"

namespace http {

const std::string SP                  = " ";
const std::string HTAB                = "\t";
const std::string OPTIONAL_WHITESPACE = SP + HTAB;
const std::string CRLF                = "\r\n";
const std::string HEADER_FIELDS_END   = CRLF + CRLF;
const std::string HTTP_VERSION        = "HTTP/1.1";
const std::string SERVER_VERSION      = "webserv/1.1";
const std::string TEXT_HTML           = "text/html";
const std::string TEXT_PLAIN           = "text/plain";

const std::string GET                       = "GET";
const std::string DELETE                    = "DELETE";
const std::string POST                      = "POST";
const std::string DEFAULT_METHODS[]         = {GET, DELETE, POST};
const std::size_t DEFAULT_METHODS_SIZE      = sizeof(DEFAULT_METHODS) / sizeof(DEFAULT_METHODS[0]);
const std::string DEFAULT_ALLOWED_METHODS[] = {GET};
const std::size_t DEFAULT_ALLOWED_METHODS_SIZE =
	sizeof(DEFAULT_ALLOWED_METHODS) / sizeof(DEFAULT_ALLOWED_METHODS[0]);

const std::string HOST                    = "Host";
const std::string USER_AGENT              = "User-Agent";
const std::string ACCEPT                  = "Accept";
const std::string ACCEPT_ENCODING         = "Accept-Encoding";
const std::string CONNECTION              = "Connection";
const std::string KEEP_ALIVE              = "keep-alive";
const std::string CLOSE                   = "close";
const std::string CONTENT_TYPE            = "Content-Type";
const std::string CONTENT_LENGTH          = "Content-Length";
const std::string TRANSFER_ENCODING       = "Transfer-Encoding";
const std::string CHUNKED                 = "chunked";
const std::string AUTHORIZATION           = "Authorization";
const std::string REQUEST_HEADER_FIELDS[] = {
	HOST,
	USER_AGENT,
	ACCEPT,
	ACCEPT_ENCODING,
	CONNECTION,
	CONTENT_TYPE,
	CONTENT_LENGTH,
	TRANSFER_ENCODING,
	AUTHORIZATION
};
const std::size_t REQUEST_HEADER_FIELDS_SIZE =
	sizeof(REQUEST_HEADER_FIELDS) / sizeof(REQUEST_HEADER_FIELDS[0]);

// response header fields
const std::string SERVER = "Server";

} // namespace http
