#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "context.hpp"
#include "custom_const_iterator.hpp"
#include "node.hpp"
#include <list>
#include <set>

namespace config {
namespace parser {

/**
 * @brief CustomConstIterator: throw on unexpected end
 */
typedef CustomConstIterator<node::Node> NodeItr;

class Parser {
  private:
	const std::list<node::Node>  &tokens_;
	std::list<context::ServerCon> servers_;
	// Prohibit Copy
	Parser(const Parser &);
	Parser &operator=(const Parser &);

	context::ServerCon   CreateServerContext(NodeItr &);
	context::LocationCon CreateLocationContext(NodeItr &);

	void ParseNode();
	void HandleServerContextDirective(context::ServerCon &, NodeItr &);
	void HandleLocationContextDirective(context::LocationCon &, NodeItr &);

	/**
	 * @brief Handlers for each Server Directive
	 */

	void HandleServerName(std::list<std::string> &server_names, NodeItr &it);
	void HandleListen(context::HostPortPair &host_port, NodeItr &it);
	void HandleClientMaxBodySize(std::size_t &client_max_body_size, NodeItr &it);
	void HandleErrorPage(std::pair<unsigned int, std::string> &error_page, NodeItr &it);

	/**
	 * @brief Handlers for each Location Directive
	 */

	void HandleAlias(std::string &alias, NodeItr &it);
	void HandleIndex(std::string &index, NodeItr &it);
	void HandleAutoIndex(bool &autoindex, NodeItr &it);
	void HandleAllowedMethods(std::list<std::string> &allowed_methods, NodeItr &it);
	void HandleReturn(std::pair<unsigned int, std::string> &redirect, NodeItr &it);
	void HandleCgiExtension(std::string &cgi_extension, NodeItr &it);
	void HandleUploadDirectory(std::string &upload_directory, NodeItr &it);

	static const int PORT_MIN        = 1024;
	static const int PORT_MAX        = 65535;
	static const int STATUS_CODE_MIN = 300;
	static const int STATUS_CODE_MAX = 599;

	/* For duplicated parameter */
	typedef std::set<std::string> DirectiveSet;
	DirectiveSet                  server_directive_set_;
	DirectiveSet                  location_directive_set_;

  public:
	explicit Parser(std::list<node::Node> &);
	~Parser();

	std::list<context::ServerCon> GetServers() const;
};

} // namespace parser
} // namespace config

#endif
