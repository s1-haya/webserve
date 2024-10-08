#include <string>

namespace utils {

bool StartWith(const std::string &str, const std::string &prefix) {
	return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}

} // namespace utils
