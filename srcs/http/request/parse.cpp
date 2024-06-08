#include "http.hpp"
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
	const std::vector<std::string> lines      = utils::SplitStr(read_buf, "\r\n");
	const std::string              start_line = lines[0];

	const std::vector<std::string> request_line = utils::SplitStr(start_line, " ");
	// set request-line(method, request-target, HTTP-version)
	request_[HTTP_METHOD]         = request_line[0];
	request_[HTTP_REQUEST_TARGET] = CreateDefaultPath(request_line[1]);
	request_[HTTP_VERSION]        = request_line[2];
}
