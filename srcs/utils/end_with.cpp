#include <string>

namespace utils {

bool EndWith(const std::string &str, const std::string &suffix) {
	return str.size() >= suffix.size() &&
		   str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

} // namespace utils
