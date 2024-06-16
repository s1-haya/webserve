#include "node.hpp"

Node::Node(const std::string &token, int token_type)
	: token_(token), token_type_(token_type) {}

Node::~Node() {}
