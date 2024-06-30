#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "node.hpp"
#include "server.hpp"
#include <list>
#include <vector>

class Parser {
  private:
	std::list<Node>     &tokens_;
	std::list<ServerCon> servers_;

  public:
	Parser(std::list<Node> &);
	~Parser();

	void ServerContext(std::list<Node>::iterator &);
	void LocationContext(std::list<Node>::iterator &, ServerCon);
};

#endif