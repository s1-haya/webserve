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
} // namespace

// todo: tmp request_
void Http::ParseRequest(const std::string &read_buf) {
	const std::vector<std::string> lines      = SplitStr(read_buf, "\r\n");
	const std::string              start_line = lines[0];
	(void)start_line;

	request_[HTTP_METHOD] = "GET";
	request_[HTTP_PATH]   = "/html/index.html";
}
