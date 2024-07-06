#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "node.hpp"
#include "server.hpp"
#include <list>
#include <vector>

namespace parser {

class Parser {
  private:
	std::list<Node>     &tokens_;
	std::list<ServerCon> servers_;
	// Prohibit Copy
	Parser(const Parser &);
	Parser     &operator=(const Parser &);
	ServerCon   ServerContext(std::list<Node>::iterator &);
	LocationCon LocationContext(std::list<Node>::iterator &);

  public:
	Parser(std::list<Node> &);
	~Parser();

	std::list<ServerCon> ReturnServers();
};

} // namespace parser

#endif