/*仮*/
#pragma once
#include <string>

class Node {
  private:
	std::string token_;
	int         token_type_;
	// Node();
  public:
	Node(std::string &token, int token_type);
	~Node();
	std::string &GetToken();
	int          GetTokenType() const;
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