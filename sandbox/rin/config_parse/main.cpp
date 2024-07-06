/*-----------------Config----------------*/

// #include "config.hpp"

// int main() {
// 	std::string conf = "config";
// 	ConfigInstance->Create(conf);
// 	std::cout << ConfigInstance->server_.server_name_ << std::endl;
// }

/*-----------------Lexer----------------*/
// #include "lexer.hpp"
// #include <iostream>

// namespace {
// 	void PrintTokens(std::list<Node> *tokens_) { /*デバッグ用*/
// 		std::list<Node> tmp = *tokens_;
// 		while (!tmp.empty()) {
// 			Node node = tmp.front();
// 			tmp.pop_front();
// 			std::cout << "token: " << node.token_ << "   "
// 					  << "token_type: " << node.token_type_ << std::endl;
// 		}
// 	}
// } // namespace

// #include <fstream>
// #include <sstream>
// // c++ main.cpp lexer.cpp node.cpp ../utils/isspace.cpp

// int main() {
// 	std::ifstream     conf("config_samp");
// 	std::stringstream ss;
// 	ss << conf.rdbuf();
// 	std::string buffer = ss.str();
// 	try {
// 		std::list<Node> *tokens_ = new std::list<Node>;
// 		Lexer            lex(buffer, *tokens_);
// 		PrintTokens(tokens_);
// 		delete tokens_;
// 	} catch (const std::exception &e) {
// 		std::cerr << e.what() << '\n';
// 	}
// }

/*-----------------Parser----------------*/
#include "lexer.hpp"
#include "parser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace {

void PrintServers(std::list<context::ServerCon> *servers_) { /*デバッグ用*/
	std::list<context::ServerCon> tmp = *servers_;
	while (!tmp.empty()) {
		context::ServerCon server = tmp.front();
		tmp.pop_front();
		std::cout << "server_name: " << server.server_name << "   "
				  << "port: ";
		for (std::list<int>::iterator it = server.port.begin(); it != server.port.end(); ++it) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;
	}
}

void PrintLocation(context::LocationCon *location_) { /*デバッグ用*/
	context::LocationCon tmp = *location_;
	std::cout << "location: " << location_->location << "   "
			  << "root: " << location_->root << "   "
			  << "index: " << location_->index << std::endl;
}

void PrintServersLocation(std::list<context::ServerCon> *servers) { /*デバッグ用*/
	std::list<context::ServerCon> tmp = *servers;
	while (!tmp.empty()) {
		context::ServerCon server = tmp.front();
		tmp.pop_front();
		std::cout << "server_name: " << server.server_name << "   "
				  << "port: ";
		for (std::list<int>::iterator it = server.port.begin(); it != server.port.end(); ++it) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;
		std::list<context::LocationCon> tmp_loc = server.location_con;
		while (!tmp_loc.empty()) {
			PrintLocation(&tmp_loc.front());
			tmp_loc.pop_front();
		}
	}
}

} // namespace

// c++ main.cpp parser.cpp lexer.cpp ../utils/isspace.cpp

int main() {
	std::ifstream     conf("config_samp");
	std::stringstream ss;
	ss << conf.rdbuf();
	std::string buffer = ss.str();
	try {
		std::list<node::Node> *tokens_ = new std::list<node::Node>;
		lexer::Lexer           lex(buffer, *tokens_);
		// PrintTokens(tokens_);
		parser::Parser                par(*tokens_);
		std::list<context::ServerCon> servers = par.ReturnServers();
		PrintServersLocation(&servers);
		delete tokens_;
		return EXIT_SUCCESS;
	} catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
}
