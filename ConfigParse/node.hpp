#ifndef NODE_HPP_
#define NODE_HPP_
#include <string>

class Node {
  private:
	std::string token_;
	int         token_type_;

  public:
	Node(std::string &token, int token_type);
	~Node();
	std::string &GetToken();
	int          GetTokenType();
};

Node::Node(std::string &token, int token_type)
	: token_(token), token_type_(token_type) {}

Node::~Node() {}

std::string &Node::GetToken() {
	return token_;
}

int Node::GetTokenType() {
	return token_type_;
}

#endif