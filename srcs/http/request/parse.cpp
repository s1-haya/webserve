#include "http.hpp"
#include "http_message.hpp"
#include "split.hpp"
#include <vector>

namespace {
	// todo: create path (/, /aaa, /aaa/)
	std::string CreateDefaultPath(const std::string &path) {
		static const std::string location = "html";

		if (path.size() == 1) {
			return location + "/index.html";
		}
		return location + path + "/index.html";
	}
} // namespace

// todo: tmp request_
void Http::ParseRequest(const std::string &read_buf) {
	const std::vector<std::string> lines      = SplitStr(read_buf, CRLF);
	const std::string              start_line = lines[0];

	const std::vector<std::string> request_line = SplitStr(start_line, SP);
	// set request-line(method, request-target, HTTP-version)
	request_[HTTP_METHOD]         = request_line[0];
	request_[HTTP_REQUEST_TARGET] = CreateDefaultPath(request_line[1]);
	request_[HTTP_VERSION]        = request_line[2];
}
