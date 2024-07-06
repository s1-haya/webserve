#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "node.hpp"
#include "server.hpp"
#include <list>

namespace parser {

class Parser {
  private:
	std::list<node::Node> &tokens_;
	std::list<ServerCon>   servers_;
	// Prohibit Copy
	Parser(const Parser &);
	Parser &operator=(const Parser &);

	ServerCon   ServerContext(std::list<node::Node>::iterator &);
	LocationCon LocationContext(std::list<node::Node>::iterator &);

  public:
	Parser(std::list<node::Node> &);
	~Parser();

	std::list<ServerCon> ReturnServers();
};

} // namespace parser

#endif
