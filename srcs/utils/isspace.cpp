#include "utils.hpp"

namespace utils {

bool IsSpace(char c) {
	if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
		return true;
	return false;
}

} // namespace utils