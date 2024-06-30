#include "utils.hpp"

namespace utils {

std::vector<std::string> SplitStr(const std::string &src, const std::string &substring) {
	std::vector<std::string> split_str;
	const std::size_t        src_size    = src.size();
	const std::size_t        substr_size = substring.size();

	if (substr_size == 0) {
		split_str.push_back(src);
		return split_str;
	}
	std::size_t start = 0;
	while (start <= src_size) {
		const std::string::size_type pos = src.find(substring, start);
		if (pos == std::string::npos) {
			split_str.push_back(src.substr(start));
			break;
		}
		split_str.push_back(src.substr(start, pos - start));
		start = pos + substr_size;
	}
	return split_str;
}

} // namespace utils
