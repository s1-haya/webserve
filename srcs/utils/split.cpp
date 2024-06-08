#include "split.hpp"

namespace utils {
	std::vector<std::string>
	SplitStr(const std::string &src, const std::string &substring) {
		std::vector<std::string> split_str;

		if (substring.size() == 0) {
			split_str.push_back(src);
			return split_str;
		}
		std::size_t start = 0;
		while (start <= src.size()) {
			const std::string::size_type pos = src.find(substring, start);
			if (pos == std::string::npos) {
				split_str.push_back(src.substr(start));
				break;
			}
			split_str.push_back(src.substr(start, pos - start));
			start = pos + substring.size();
		}
		return split_str;
	}
} // namespace utils
