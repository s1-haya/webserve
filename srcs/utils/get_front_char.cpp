#include <string>

namespace utils {

// std::string::front()
char GetFrontChar(const std::string &str) {
	return str.empty() ? '\0' : str[0];
}

} // namespace utils
