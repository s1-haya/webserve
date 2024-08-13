#include "parser.hpp"
#include "directive_names.hpp"
#include <cstdlib> // atoi
#include <stdexcept>

namespace config {
namespace parser {

Parser::Parser(std::list<node::Node> &tokens) : tokens_(tokens) {
	ParseNode();
}

Parser::~Parser() {}

void Parser::ParseNode() {
	for (NodeItr it = tokens_.begin(); it != tokens_.end(); ++it) {
		if ((*it).token_type == node::CONTEXT && (*it).token == SERVER) {
			servers_.push_back(CreateServerContext(++it));
		} else {
			throw std::runtime_error("expect server context");
		}
	}
}

/**
 * @brief Handlers for Server Context
 * @details Handlers for each Server Directive
 */

context::ServerCon Parser::CreateServerContext(NodeItr &it) {
	context::ServerCon server;

	if ((*it).token_type != node::L_BRACKET) {
		throw std::runtime_error("expect { after server");
	}
	++it; // skip L_BRACKET
	while (it != tokens_.end() && (*it).token_type != node::R_BRACKET) {
		switch ((*it).token_type) {
		case node::DIRECTIVE:
			HandleServerContextDirective(server, it);
			break;
		case node::CONTEXT:
			if ((*it).token == LOCATION) {
				server.location_con.push_back(CreateLocationContext(++it));
			} else {
				throw std::runtime_error("invalid nest of 'server' directive");
			}
			break;
		case node::DELIM:
			throw std::runtime_error("unexpected ';'");
			break;
		default:
			throw std::runtime_error("unknown token");
			break;
		}
		++it;
	}
	if (it == tokens_.end()) {
		throw std::runtime_error("expect }");
	}
	return server;
}

void Parser::HandleServerContextDirective(context::ServerCon &server, NodeItr &it) {
	if ((*it).token == SERVER_NAME) {
		HandleServerName(server.server_names, ++it);
	} else if ((*it).token == LISTEN) {
		HandleListen(server.port, ++it);
	} else if ((*it).token == CLIENT_MAX_BODY_SIZE) {
		HandleClientMaxBodySize(server.client_max_body_size, ++it);
	} else if ((*it).token == ERROR_PAGE) {
		HandleErrorPage(server.error_page, ++it);
	}
	if ((*it).token_type != node::DELIM) {
		throw std::runtime_error("expect ';' after arguments");
	}
}

void Parser::HandleServerName(std::list<std::string> &server_names, NodeItr &it) {
	while ((*it).token_type != node::DELIM && (*it).token_type == node::WORD) {
		server_names.push_back((*it++).token);
		if (it == tokens_.end()) {
			throw std::runtime_error("invalid arguments of 'server_name' directive");
		}
	}
}

void Parser::HandleListen(context::PortList &port, NodeItr &it) {
	if ((*it).token_type != node::WORD) {
		throw std::runtime_error("invalid number of arguments in 'listen' directive");
	}
	port.push_back(std::atoi((*it++).token.c_str())); // TODO: atoi, validation, 重複チェック
}

void Parser::HandleClientMaxBodySize(std::size_t &client_max_body_size, NodeItr &it) {
	if ((*it).token_type != node::WORD) {
		throw std::runtime_error("invalid number of arguments in 'client_max_body_size' directive");
	}
	client_max_body_size = std::atoi((*it++).token.c_str()); // tmp: atoi
}

void Parser::HandleErrorPage(std::pair<unsigned int, std::string> &error_page, NodeItr &it) {
	if ((*it).token_type != node::WORD || (*++NodeItr(it)).token_type != node::WORD) {
		throw std::runtime_error("invalid number of arguments in 'error_page' directive");
	}
	// ex. 404 /404.html, tmp: atoi
	NodeItr tmp_it = it; // 404
	it++;                // /404.html
	error_page = std::make_pair(std::atoi((*tmp_it).token.c_str()), (*it).token);
	it++;
}

/**
 * @brief Handlers for Location Context
 * @details Handlers for each Location Directive
 */

context::LocationCon Parser::CreateLocationContext(NodeItr &it) {
	context::LocationCon location;

	if ((*it).token_type != node::WORD) {
		throw std::runtime_error("invalid number of arguments in 'location' directive");
	}
	location.request_uri = (*it).token;
	++it; // skip /www/
	if ((*it).token_type != node::L_BRACKET) {
		throw std::runtime_error("expect { after location argument");
	}
	++it; // skip L_BRACKET
	while (it != tokens_.end() && (*it).token_type != node::R_BRACKET) {
		switch ((*it).token_type) {
		case node::DIRECTIVE:
			HandleLocationContextDirective(location, it);
			break;
		case node::DELIM:
			throw std::runtime_error("unexpected ';'");
			break;
		case node::CONTEXT:
			throw std::runtime_error("invalid nest of 'location' directive"); // nginxではok
			break;
		default:
			throw std::runtime_error("unknown token");
			break;
		}
		++it;
	}
	if (it == tokens_.end()) {
		throw std::runtime_error("expect }");
	}
	return location;
}

void Parser::HandleLocationContextDirective(context::LocationCon &location, NodeItr &it) {
	if ((*it).token == ALIAS) {
		HandleAlias(location.alias, ++it);
	} else if ((*it).token == INDEX) {
		HandleIndex(location.index, ++it);
	} else if ((*it).token == AUTO_INDEX) {
		HandleAutoIndex(location.autoindex, ++it);
	} else if ((*it).token == ALLOWED_METHODS) {
		HandleAllowedMethods(location.allowed_methods, ++it);
	} else if ((*it).token == RETURN) {
		HandleReturn(location.redirect, ++it);
	}
	if ((*it).token_type != node::DELIM) {
		throw std::runtime_error("expect ';' after arguments");
	}
}

void Parser::HandleAlias(std::string &alias, NodeItr &it) {
	if ((*it).token_type != node::WORD) {
		throw std::runtime_error("invalid number of arguments in 'alias' directive");
	}
	alias = (*it++).token;
}

void Parser::HandleIndex(std::string &index, NodeItr &it) {
	if ((*it).token_type != node::WORD) {
		throw std::runtime_error("invalid number of arguments in 'index' directive");
	}
	index = (*it++).token;
}

void Parser::HandleAutoIndex(bool &autoindex, NodeItr &it) {
	if ((*it).token_type != node::WORD || ((*it).token != "on" && (*it).token != "off")) {
		throw std::runtime_error("invalid arguments in 'autoindex' directive");
	}
	autoindex = ((*it++).token == "on");
}

void Parser::HandleAllowedMethods(std::list<std::string> &allowed_methods, NodeItr &it) {
	while ((*it).token_type != node::DELIM && (*it).token_type == node::WORD) {
		allowed_methods.push_back((*it++).token);
		if (it == tokens_.end()) {
			throw std::runtime_error("invalid arguments of 'allowed_methods' directive");
		}
	}
}

void Parser::HandleReturn(std::pair<unsigned int, std::string> &redirect, NodeItr &it) {
	if ((*it).token_type != node::WORD || (*++NodeItr(it)).token_type != node::WORD) {
		throw std::runtime_error("invalid number of arguments in 'return' directive");
	}
	// ex. 302 /index.html, tmp: atoi
	NodeItr tmp_it = it; // 302
	it++;                // /index.html
	redirect = std::make_pair(std::atoi((*tmp_it).token.c_str()), (*it).token);
	it++;
}

std::list<context::ServerCon> Parser::GetServers() const {
	return this->servers_;
}

} // namespace parser
} // namespace config
