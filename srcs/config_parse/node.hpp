#ifndef NODE_HPP_
#define NODE_HPP_
#include <string>

namespace node {

struct Node {
	std::string token;
	int         token_type;
};

enum TokenType {
	DELIM,
	L_BRACKET,
	R_BRACKET,
	CONTEXT,
	DIRECTIVE,
	WORD
};

} // namespace node

#endif
