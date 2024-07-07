#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "context.hpp"
#include "node.hpp"
#include <list>

namespace parser {

typedef std::list<node::Node>::iterator NodeItr;

class Parser {
  private:
	std::list<node::Node>        &tokens_;
	std::list<context::ServerCon> servers_;
	// Prohibit Copy
	Parser(const Parser &);
	Parser &operator=(const Parser &);

	context::ServerCon   ServerContext(NodeItr &);
	context::LocationCon LocationContext(NodeItr &);

  public:
	Parser(std::list<node::Node> &);
	~Parser();

	std::list<context::ServerCon> ReturnServers();
};

} // namespace parser

#endif
