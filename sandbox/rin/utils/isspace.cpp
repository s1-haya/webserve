#include "utils.hpp"

namespace utils {

bool IsSpace(char c) {
	const char CR = '\r';
	const char LF = '\n';

	if (c == ' ' || c == '\t' || c == CR || c == LF)
		return true;
	return false;
}

} // namespace utils