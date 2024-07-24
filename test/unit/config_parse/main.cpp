/*-----------------Parser----------------*/
#include "../../../srcs/config_parse/lexer.hpp"
#include "../../../srcs/config_parse/parser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace config {
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
} // namespace config

// c++ main.cpp ../../../srcs/config_parse/parser.cpp ../../../srcs/config_parse/lexer.cpp
// ../../../srcs/utils/isspace.cpp
using namespace config;

int main() {
	std::ifstream     conf("config_samp.conf");
	std::stringstream ss;
	ss << conf.rdbuf();
	std::string buffer = ss.str();
	try {
		std::list<config::node::Node> *tokens_ = new std::list<node::Node>;
		lexer::Lexer                   lex(buffer, *tokens_);
		// PrintTokens(tokens_);
		parser::Parser                par(*tokens_);
		std::list<context::ServerCon> servers = par.GetServers();
		PrintServersLocation(&servers);
		delete tokens_;
		return EXIT_SUCCESS;
	} catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
}
