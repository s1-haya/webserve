#include "split.hpp"

std::vector<std::string> SplitStr(const std::string &str, const std::string &delim) {
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
