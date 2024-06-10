#ifndef NODE_HPP_
#define NODE_HPP_
#include <string>

class Node {
  private:
	std::string token_;
	int         token_type_;

  public:
	Node(const std::string &token, int token_type);
	~Node();
	std::string GetToken() const;
	int         GetTokenType() const;
};

#endif