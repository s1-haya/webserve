#include "parser.hpp"
#include <cstdlib> // atoi
#include <stdexcept>

namespace config {
namespace parser {
namespace {

void HandleServerContextDirective(context::ServerCon &server, NodeItr &it) {
	if ((*it).token == "server_name") {
		++it;
		if ((*it).token_type != node::WORD) {
			throw std::runtime_error("invalid number of arguments in 'server_name' directive");
		}
		server.server_name = (*it++).token; // TODO: 複数の名前
	} else if ((*it).token == "listen") {
		++it;
		if ((*it).token_type != node::WORD) {
			throw std::runtime_error("invalid number of arguments in 'listen' directive");
		}
		server.port.push_back(std::atoi((*it++).token.c_str())
		); // TODO: atoi, validation, 重複チェック
	}
	if ((*it).token_type != node::DELIM)
		throw std::runtime_error("expect ';' after arguments");
}

void HandleLocationContextDirective(context::LocationCon &location, NodeItr &it) {
	if ((*it).token == "root") {
		++it;
		if ((*it).token_type != node::WORD)
			throw std::runtime_error("invalid number of arguments in 'root' directive");
		location.root = (*it++).token;
	} else if ((*it).token == "index") {
		++it;
		if ((*it).token_type != node::WORD)
			throw std::runtime_error("invalid number of arguments in 'index' directive");
		location.index = (*it++).token;
	}
	if ((*it).token_type != node::DELIM)
		throw std::runtime_error("expect ';' after arguments");
}

} // namespace

Parser::Parser(std::list<node::Node> &tokens) : tokens_(tokens) {
	for (NodeItr it = tokens_.begin(); it != tokens_.end(); ++it) {
		if ((*it).token_type == node::CONTEXT && (*it).token == "server") {
			servers_.push_back(CreateServerContext(++it));
		} else {
			throw std::runtime_error("expect server context");
		}
	}
}

Parser::~Parser() {}

context::ServerCon Parser::CreateServerContext(NodeItr &it) {
	context::ServerCon server;

	if ((*it).token_type != node::L_BRACKET)
		throw std::runtime_error("expect { after server");
	++it; // skip L_BRACKET
	while (it != tokens_.end() && (*it).token_type != node::R_BRACKET) {
		switch ((*it).token_type) {
		case node::DIRECTIVE:
			HandleServerContextDirective(server, it);
			break;
		case node::CONTEXT:
			if ((*it).token == "location")
				server.location_con.push_back(CreateLocationContext(++it));
			else
				throw std::runtime_error("invalid nest of 'server' directive");
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
	if (it == tokens_.end())
		throw std::runtime_error("expect }");
	return server;
}

context::LocationCon Parser::CreateLocationContext(NodeItr &it) {
	context::LocationCon location;

	if ((*it).token_type != node::WORD)
		throw std::runtime_error("invalid number of arguments in 'location' directive");
	location.location = (*it).token;
	++it; // skip /www/
	if ((*it).token_type != node::L_BRACKET)
		throw std::runtime_error("expect { after location argument");
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
	if (it == tokens_.end())
		throw std::runtime_error("expect }");
	return location;
}

std::list<context::ServerCon> Parser::GetServers() const {
	return this->servers_;
}

} // namespace parser
} // namespace config
