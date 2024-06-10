#include "node.hpp"

Node::Node(std::string &token, int token_type)
	: token_(token), token_type_(token_type) {}

Node::~Node() {}

std::string &Node::GetToken() {
	return token_;
}

int Node::GetTokenType() {
	return token_type_;
}
