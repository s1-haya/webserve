#include "parser.hpp"

namespace {

void HandleServerContextDirective(ServerCon &server, std::list<Node>::iterator &it) {
	if ((*it).token_ == "server_name") {
		++it;
		if ((*it).token_type_ != WORD)
			throw std::logic_error("invalid number of arguments in 'server_name' directive");
		server.server_name_ = (*it).token_; // TODO: 複数の名前
	} else if ((*it).token_ == "listen") {
		++it;
		if ((*it).token_type_ != WORD)
			throw std::logic_error("invalid number of arguments in 'listen' directive");
		server.port_ = std::atoi((*it).token_.c_str()); // TODO: atoi、複数Listen
	}
}

void HandleLocationContextDirective(LocationCon &location, std::list<Node>::iterator &it) {
	if ((*it).token_ == "root") {
		++it;
		if ((*it).token_type_ != WORD)
			throw std::logic_error("invalid number of arguments in 'root' directive");
		location.root_ = (*it).token_;
	} else if ((*it).token_ == "index") {
		++it;
		if ((*it).token_type_ != WORD)
			throw std::logic_error("invalid number of arguments in 'index' directive");
		location.index_ = (*it).token_;
	}
}

} // namespace

Parser::Parser(std::list<Node> &tokens) : tokens_(tokens) {
	for (std::list<Node>::iterator it = tokens_.begin(); it != tokens_.end(); ++it) {
		if ((*it).token_type_ == CONTEXT && (*it).token_ == "server") {
			servers_.push_back(ServerContext(++it));
		}
	}
}

Parser::~Parser() {}

ServerCon Parser::ServerContext(std::list<Node>::iterator &it) {
	ServerCon server;

	if ((*it).token_type_ != L_BRACKET)
		throw std::logic_error("expect {");
	++it; // skip L_BRACKET
	while ((*it).token_type_ != R_BRACKET && it != tokens_.end()) {
		switch ((*it).token_type_) {
		case DIRECTIVE:
			HandleServerContextDirective(server, it);
			break;
		case CONTEXT:
			if ((*it).token_ == "location")
				server.location_con_.push_back(LocationContext(++it));
			else
				throw std::logic_error("invalid nest of 'server' directive");
			break;
		case DELIM:
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

	if ((*it).token_type_ != WORD)
		throw std::logic_error("invalid number of arguments in 'location' directive");
	location.location_ = (*it).token_;
	++it; // skip /www/
	if ((*it).token_type_ != L_BRACKET)
		throw std::logic_error("expect {");
	++it; // skip L_BRACKET
	while ((*it).token_type_ != R_BRACKET && it != tokens_.end()) {
		switch ((*it).token_type_) {
		case DIRECTIVE:
			HandleLocationContextDirective(location, it);
			break;
		case DELIM:
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
