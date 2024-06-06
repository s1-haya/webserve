#include "lexer.hpp"
#include <vector>

/*統合時外す*/
#define LF '\n'
#define CR '\r'
/*---*/

/*仮*/
enum {
	CONF_ERROR,
	CONF_OK,
	CONF_BLOCK_START,
	CONF_BLOCK_DONE,
	CONF_FILE_DONE,
};
/*---*/

namespace {
	std::vector<std::string>
	SplitStr(const std::string &str, const std::string delim) {
		std::vector<std::string> split_str;

		std::size_t start = 0;
		while (start < str.size()) {
			const std::string::size_type pos = str.find(delim, start);
			if (pos == std::string::npos) {
				split_str.push_back(str.substr(start));
				break;
			}
			split_str.push_back(str.substr(start, pos - start));
			start = pos + delim.size();
		}
		return split_str;
	}
} // namespace

Lexer::Lexer(std::string &buffer) {}

Lexer::~Lexer() {}

/*namespaceに入れる*/

static bool IsSpace(char c) {
	if (c == ' ' || c == '\t' || c == CR || c == LF)
		return true;
	return false;
}

static int LexerReadToken(const std::string &buffer) {
	bool need_space    = false;
	bool last_space    = true;
	bool sharp_comment = false;
	bool found         = false;

	int         line_count = 0;
	std::string data;

	for (std::string::const_iterator it = buffer.begin() + 1; it != buffer.end();
		 ++it) {
		if (*it == LF) {
			++line_count;
			if (sharp_comment)
				sharp_comment = false;
		}
		if (sharp_comment)
			continue;
		// // if (quoted)
		// if (need_space)
		// {
		// 	if (isspace(*it)) {
		// 		last_space = true;
		// 		need_space = false;
		// 		continue;
		// 	}
		// 	if (*it == ';')
		// 		return CONF_OK;
		// 	if (*it == '{')
		// 		return CONF_BLOCK_START;
		// }
		if (last_space) {
			char start = *(it - 1);
			if (IsSpace(*it))
				continue;
			switch (*it) {
			case ';':
			case '{':
				return CONF_BLOCK_START;
			case '}':
				return CONF_BLOCK_DONE;
			case '#':
				sharp_comment = true;
				continue;
			default:
				last_space = false;
			}
		} else {
			if (IsSpace(*it) || *it == ';' || *it == '{') {
				last_space = true;
				found      = true;
			}
			// if (found)
			// {

			// }
		}
	}
}

/*----------*/

/*テスト用*/

#include <sstream>

int main() {
	std::fstream      fs("config");
	std::stringstream ss;
	ss << fs.rdbuf();
	LexerReadToken(ss.str());
}
