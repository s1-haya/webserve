#include "utils.hpp"

namespace utils {

// https://en.cppreference.com/w/cpp/string/byte/isdigit
bool IsDigit(char c) {
	return std::isdigit(static_cast<unsigned char>(c));
}

} // namespace utils
