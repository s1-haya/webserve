#ifndef NODE_HPP_
#define NODE_HPP_
#include <string>

struct Node {
	std::string token_;
	int         token_type_;
	Node(const std::string &token, int token_type);
	~Node();
};

#endif