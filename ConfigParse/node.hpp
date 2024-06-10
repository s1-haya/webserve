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

#endif