#include "parser.hpp"

Parser::Parser(std::list<Node> &tokens) : tokens_(tokens) {
	for (std::list<Node>::iterator it = tokens_.begin(); it != tokens_.end(); ++it) {
		if ((*it).token_type_ == CONTEXT && (*it).token_ == "server") {
			ServerCon server;
			ServerContext(++it);
		}
	}
}

Parser::~Parser() {}

ServerCon Parser::ServerContext(std::list<Node>::iterator &it) {
	ServerCon server;

	if ((*it).token_type_ != L_BRACKET)
		throw std::logic_error("no {");
	while ((*it).token_type_ != R_BRACKET && it != tokens_.end()) {
		switch ((*it).token_type_) {
		case DIRECTIVE:
			if ((*it).token_ == "server_name") {
				++it;
				if ((*it).token_type_ != WORD)
					throw std::logic_error("server_name"); // 仮
				server.server_name_ = (*it).token_;        // TODO: 複数の名前
			} else if ((*it).token_ == "listen") {
				++it;
				if ((*it).token_type_ != WORD)
					throw std::logic_error("server_name");      // 仮
				server.port_ = std::atoi((*it).token_.c_str()); // TODO: atoi
			}
		case CONTEXT:
			if ((*it).token_ == "server")
				ServerContext(it);
			else if ((*it).token_ == "location")
				LocationContext(it);
		case DELIM:
			break;
		default:
			throw std::logic_error("unknown token");
			break;
		}
		// if !} throw err
		++it;
	}
	servers_.push_back(server);
}

LocationCon LocationContext(std::list<Node>::iterator &it) {}
