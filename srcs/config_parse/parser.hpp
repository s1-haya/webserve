#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "context.hpp"
#include "node.hpp"
#include <list>

namespace config {
namespace parser {

typedef std::list<node::Node>::iterator NodeItr;

class Parser {
  private:
	std::list<node::Node>        &tokens_;
	std::list<context::ServerCon> servers_;
	// Prohibit Copy
	Parser(const Parser &);
	Parser &operator=(const Parser &);

	context::ServerCon   CreateServerContext(NodeItr &);
	context::LocationCon CreateLocationContext(NodeItr &);

	void ParseNode();
	void HandleServerContextDirective(context::ServerCon &server, NodeItr &it);
	void HandleLocationContextDirective(context::LocationCon &location, NodeItr &it);

	void HandleServerName(std::list<std::string> &server_names, NodeItr &it);
	void HandleListen(context::PortList &ports, NodeItr &it);
	void HandleClientMaxBodySize(std::size_t &client_max_body_size, NodeItr &it);
	void HandleErrorPage(std::pair<unsigned int, std::string> &error_page, NodeItr &it);

	void HandleAlias(std::string &alias, NodeItr &it);
	void HandleIndex(std::string &index, NodeItr &it);
	void HandleAutoIndex(bool &autoindex, NodeItr &it);
	void HandleAllowedMethods(std::list<std::string> &allowed_methods, NodeItr &it);
	void HandleReturn(std::pair<unsigned int, std::string> &redirect, NodeItr &it);

  public:
	Parser(std::list<node::Node> &);
	~Parser();

	std::list<context::ServerCon> GetServers() const;
};

} // namespace parser
} // namespace config

#endif
