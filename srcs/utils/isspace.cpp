#include "isspace.hpp"

namespace utils {
	bool IsSpace(char c) {
		if (c == ' ' || c == '\t' || c == utils::CR || c == utils::LF)
			return true;
		return false;
	}
} // namespace utils