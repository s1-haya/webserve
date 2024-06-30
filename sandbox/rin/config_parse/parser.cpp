#include "parser.hpp"

namespace {

void PrintServers(std::list<ServerCon> *servers_) { /*デバッグ用*/
	std::list<ServerCon> tmp = *servers_;
	while (!tmp.empty()) {
		ServerCon server = tmp.front();
		tmp.pop_front();
		std::cout << "server_name: " << server.server_name_ << "   "
				  << "port: " << server.port_ << std::endl;
	}
}
void PrintLocation(LocationCon *location_) { /*デバッグ用*/
	LocationCon tmp = *location_;
	std::cout << "location: " << location_->location_ << "   "
			  << "root: " << location_->root_ << "   "
			  << "index: " << location_->index_ << std::endl;
}

void PrintServersLocation(std::list<ServerCon> *servers_) { /*デバッグ用*/
	std::list<ServerCon> tmp = *servers_;
	while (!tmp.empty()) {
		ServerCon server = tmp.front();
		tmp.pop_front();
		std::cout << "server_name: " << server.server_name_ << "   "
				  << "port: " << server.port_ << "   " << std::endl;
		std::list<LocationCon> tmp_loc = server.location_con_;
		while (!tmp_loc.empty()) {
			PrintLocation(&tmp_loc.front());
			tmp_loc.pop_front();
		}
	}
}
} // namespace

Parser::Parser(std::list<Node> &tokens) : tokens_(tokens) {
	for (std::list<Node>::iterator it = tokens_.begin(); it != tokens_.end(); ++it) {
		if ((*it).token_type_ == CONTEXT && (*it).token_ == "server") {
			ServerContext(++it);
		}
	}
	PrintServersLocation(&servers_);
}

Parser::~Parser() {}

void Parser::ServerContext(std::list<Node>::iterator &it) {
	ServerCon server;

	if ((*it).token_type_ != L_BRACKET)
		throw std::logic_error("no {");
	++it; // skip L_BRACKET
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
					throw std::logic_error("listen");           // 仮
				server.port_ = std::atoi((*it).token_.c_str()); // TODO: atoi
			}
		case CONTEXT:
			if ((*it).token_ == "server")
				ServerContext(++it);
			else if ((*it).token_ == "location")
				LocationContext(++it, &server);
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

void Parser::LocationContext(std::list<Node>::iterator &it, ServerCon *server) {
	LocationCon location;

	if ((*it).token_type_ != WORD)
		throw std::logic_error("no after location");
	location.location_ = (*it).token_;
	++it; // skip /www/
	if ((*it).token_type_ != L_BRACKET)
		throw std::logic_error("no {");
	++it; // skip L_BRACKET
	while ((*it).token_type_ != R_BRACKET && it != tokens_.end()) {
		switch ((*it).token_type_) {
		case DIRECTIVE:
			if ((*it).token_ == "root") {
				++it;
				if ((*it).token_type_ != WORD)
					throw std::logic_error("root"); // 仮
				location.root_ = (*it).token_;
			} else if ((*it).token_ == "index") {
				++it;
				if ((*it).token_type_ != WORD)
					throw std::logic_error("index"); // 仮
				location.index_ = (*it).token_;
			}
		case DELIM:
			break;
		default: // CONTEXT?
			throw std::logic_error("unknown token");
			break;
		}
		// if !} throw err
		++it;
	}
	// PrintLocation(&location);
	server->location_con_.push_back(location);
}

#include "lexer.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
// c++ parser.cpp lexer.cpp node.cpp ../utils/isspace.cpp

int main() {
	std::ifstream     conf("config_samp.conf");
	std::stringstream ss;
	ss << conf.rdbuf();
	std::string buffer = ss.str();
	try {
		std::list<Node> *tokens_ = new std::list<Node>;
		Lexer            lex(buffer, *tokens_);
		// PrintTokens(tokens_);
		Parser par(*tokens_);
		delete tokens_;
	} catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
	}
}
