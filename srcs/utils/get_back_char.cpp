#include <string>

namespace utils {

// std::string::back()
char GetBackChar(const std::string &str) {
	return str.empty() ? '\0' : str[str.size() - 1];
}

} // namespace utils
