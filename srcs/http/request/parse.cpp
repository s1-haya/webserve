#include "http.hpp"
#include <vector>

namespace {
	std::vector<std::string>
	SplitStr(const std::string &str, const std::string delim) {
		std::vector<std::string> split_str;

		std::size_t start = 0;
		while (start < str.size()) {
			const std::string::size_type pos = str.find(delim, start);
			if (pos == std::string::npos) {
				split_str.push_back(str.substr(start));
				break;
			}
			split_str.push_back(str.substr(start, pos - start));
			start = pos + delim.size();
		}
		return split_str;
	}

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
	const std::vector<std::string> lines      = SplitStr(read_buf, "\r\n");
	const std::string              start_line = lines[0];

	const std::vector<std::string> request_line = SplitStr(start_line, " ");
	// set request-line(method, request-target, HTTP-version)
	request_[HTTP_METHOD]         = request_line[0];
	request_[HTTP_REQUEST_TARGET] = CreateDefaultPath(request_line[1]);
	request_[HTTP_VERSION]        = request_line[2];
}
