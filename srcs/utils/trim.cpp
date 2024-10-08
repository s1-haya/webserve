#include <string>

namespace utils {

std::string Trim(const std::string &str, const std::string &to_trim) {
	std::size_t first = str.find_first_not_of(to_trim);
	if (first == std::string::npos) {
		return "";
	}
	std::size_t last = str.find_last_not_of(to_trim);
	return str.substr(first, last - first + 1);
}

} // namespace utils
