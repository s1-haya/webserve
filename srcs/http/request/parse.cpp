#include "http.hpp"
#include <vector>

namespace {
	std::vector<std::string>
	SplitStr(const std::string &str, const std::string delim) {
		std::vector<std::string> split_str;

		std::string::const_iterator it = str.begin();
		while (it != str.end()) {
			const std::string      head = &(*it);
			std::string::size_type pos  = head.find(delim);
			if (pos == std::string::npos) {
				split_str.push_back(head.substr(0));
				break;
			}
			split_str.push_back(head.substr(0, pos));
			std::advance(it, pos);
			++it;
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
	// set request-line(method, request-target)
	request_[HTTP_METHOD] = request_line[0];
	request_[HTTP_PATH]   = CreateDefaultPath(request_line[1]);
}
