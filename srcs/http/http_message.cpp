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
const std::string TEXT_PLAIN          = "text/plain";

const std::string GET                       = "GET";
const std::string DELETE                    = "DELETE";
const std::string POST                      = "POST";
const std::string DEFAULT_METHODS[]         = {GET, DELETE, POST};
const std::size_t DEFAULT_METHODS_SIZE      = sizeof(DEFAULT_METHODS) / sizeof(DEFAULT_METHODS[0]);
const std::string DEFAULT_ALLOWED_METHODS[] = {GET};
const std::size_t DEFAULT_ALLOWED_METHODS_SIZE =
	sizeof(DEFAULT_ALLOWED_METHODS) / sizeof(DEFAULT_ALLOWED_METHODS[0]);

const std::string HOST                     = "host";
const std::string USER_AGENT               = "user-agent";
const std::string ACCEPT                   = "accept";
const std::string ACCEPT_ENCODING          = "accept-encoding";
const std::string CONNECTION               = "connection";
const std::string KEEP_ALIVE               = "keep-alive";
const std::string CLOSE                    = "close";
const std::string CONTENT_TYPE             = "content-type";
const std::string APPLICATION_OCTET_STREAM = "application/octet-stream";
const std::string MULTIPART_FORM_DATA      = "multipart/form-data";
const std::string BOUNDARY                 = "boundary";
const std::string CONTENT_LENGTH           = "content-length";
const std::string TRANSFER_ENCODING        = "transfer-encoding";
const std::string CHUNKED                  = "chunked";
const std::string LOCATION                 = "location";
const std::string AUTHORIZATION            = "authorization";
const std::string REQUEST_HEADER_FIELDS[]  = {
	 HOST,
	 USER_AGENT,
	 ACCEPT,
	 ACCEPT_ENCODING,
	 CONNECTION,
	 CONTENT_TYPE,
	 CONTENT_LENGTH,
	 TRANSFER_ENCODING,
	 LOCATION,
	 AUTHORIZATION};
const std::size_t REQUEST_HEADER_FIELDS_SIZE =
	sizeof(REQUEST_HEADER_FIELDS) / sizeof(REQUEST_HEADER_FIELDS[0]);

// response header fields
const std::string SERVER = "server";

// For multipart/form-data header fields
const std::string CONTENT_DISPOSITION = "Content-Disposition";
const std::string FILENAME            = "filename";

} // namespace http
