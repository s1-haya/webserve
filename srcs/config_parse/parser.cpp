#include "parser.hpp"
#include <cstdlib> // atoi

namespace parser {

namespace {

void HandleServerContextDirective(ServerCon &server, std::list<Node>::iterator &it) {
	if ((*it).token == "server_name") {
		++it;
		if ((*it).token_type != WORD)
			throw std::logic_error("invalid number of arguments in 'server_name' directive");
		server.server_name = (*it++).token; // TODO: 複数の名前
	} else if ((*it).token == "listen") {
		++it;
		if ((*it).token_type != WORD)
			throw std::logic_error("invalid number of arguments in 'listen' directive");
		while ((*it).token_type == WORD) {
			server.port.push_back(std::atoi((*it++).token.c_str())
			); // TODO: atoi, validation, 重複チェック
		}
	}
	if ((*it).token_type != DELIM)
		throw std::logic_error("expect ';' after arguments");
}

void HandleLocationContextDirective(LocationCon &location, std::list<Node>::iterator &it) {
	if ((*it).token == "root") {
		++it;
		if ((*it).token_type != WORD)
			throw std::logic_error("invalid number of arguments in 'root' directive");
		location.root = (*it++).token;
	} else if ((*it).token == "index") {
		++it;
		if ((*it).token_type != WORD)
			throw std::logic_error("invalid number of arguments in 'index' directive");
		location.index = (*it++).token;
	}
	if ((*it).token_type != DELIM)
		throw std::logic_error("expect ';' after arguments");
}

} // namespace

Parser::Parser(std::list<Node> &tokens) : tokens_(tokens) {
	for (std::list<Node>::iterator it = tokens_.begin(); it != tokens_.end(); ++it) {
		if ((*it).token_type == CONTEXT && (*it).token == "server") {
			servers_.push_back(ServerContext(++it));
		}
	}
}

Parser::~Parser() {}

ServerCon Parser::ServerContext(std::list<Node>::iterator &it) {
	ServerCon server;

	if ((*it).token_type != L_BRACKET)
		throw std::logic_error("expect { after server");
	++it; // skip L_BRACKET
	while (it != tokens_.end() && (*it).token_type != R_BRACKET) {
		switch ((*it).token_type) {
		case DIRECTIVE:
			HandleServerContextDirective(server, it);
			break;
		case CONTEXT:
			if ((*it).token == "location")
				server.location_con.push_back(LocationContext(++it));
			else
				throw std::logic_error("invalid nest of 'server' directive");
			break;
		case DELIM:
			throw std::logic_error("unexpected ';'");
			break;
		default:
			throw std::logic_error("unknown token");
			break;
		}
		++it;
	}
	if (it == tokens_.end())
		throw std::logic_error("expect }");
	return server;
}

LocationCon Parser::LocationContext(std::list<Node>::iterator &it) {
	LocationCon location;

	if ((*it).token_type != WORD)
		throw std::logic_error("invalid number of arguments in 'location' directive");
	location.location = (*it).token;
	++it; // skip /www/
	if ((*it).token_type != L_BRACKET)
		throw std::logic_error("expect { after location argument");
	++it; // skip L_BRACKET
	while (it != tokens_.end() && (*it).token_type != R_BRACKET) {
		switch ((*it).token_type) {
		case DIRECTIVE:
			HandleLocationContextDirective(location, it);
			break;
		case DELIM:
			throw std::logic_error("unexpected ';'");
			break;
		case CONTEXT:
			throw std::logic_error("invalid nest of 'location' directive"); // nginxではok
			break;
		default:
			throw std::logic_error("unknown token");
			break;
		}
		++it;
	}
	if (it == tokens_.end())
		throw std::logic_error("expect }");
	return location;
}

std::list<ServerCon> Parser::ReturnServers() {
	return this->servers_;
}

} // namespace parser
