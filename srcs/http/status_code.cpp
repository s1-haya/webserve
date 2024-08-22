#include "status_code.hpp"

namespace http {

ReasonPhrase InitReasonPhrase() {
	ReasonPhrase init_reason_phrase;
	init_reason_phrase[OK]                    = "OK";
	init_reason_phrase[MOVED_PERMANENTLY]     = "Moved Permanently";
	init_reason_phrase[BAD_REQUEST]           = "Bad Request";
	init_reason_phrase[FORBIDDEN]             = "Forbidden";
	init_reason_phrase[NOT_FOUND]             = "Not Found";
	init_reason_phrase[INTERNAL_SERVER_ERROR] = "Internal Server Error";
	init_reason_phrase[NOT_IMPLEMENTED]       = "Not Implemented";
	return init_reason_phrase;
}

const ReasonPhrase reason_phrase = InitReasonPhrase();

} // namespace http
