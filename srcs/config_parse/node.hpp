#ifndef NODE_HPP_
#define NODE_HPP_
#include <string>

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

#endif
