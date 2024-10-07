#include <cctype> // isprint
#include <string>

namespace utils {

// https://en.cppreference.com/w/cpp/string/byte/isprint
bool IsPrint(char c) {
	return std::isprint(static_cast<unsigned char>(c));
}

bool IsVString(const std::string &str) {
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (!IsPrint(*it)) {
			return false;
		}
	}
	return true;
}

} // namespace utils
