#include "http_response.hpp"

namespace http {

std::string HttpResponse::TmpRun(int client_fd) {
	(void)client_fd;
	return ("RESPONSE");
}

} // namespace http
