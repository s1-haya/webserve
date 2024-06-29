// #include "config.hpp"

// int main() {
// 	std::string conf = "config";
// 	ConfigInstance->Create(conf);
// 	std::cout << ConfigInstance->server_.server_name_ << std::endl;
// }

#include "lexer.hpp"
#include <iostream>

namespace {
	void PrintTokens(std::list<Node> *tokens_) { /*デバッグ用*/
		std::list<Node> tmp = *tokens_;
		while (!tmp.empty()) {
			Node node = tmp.front();
			tmp.pop_front();
			std::cout << "token: " << node.token_ << "   "
					  << "token_type: " << node.token_type_ << std::endl;
		}
	}
} // namespace

#include <fstream>
#include <sstream>
// c++ main.cpp lexer.cpp node.cpp ../utils/isspace.cpp

int main() {
	std::ifstream     conf("config_samp");
	std::stringstream ss;
	ss << conf.rdbuf();
	std::string buffer = ss.str();
	try {
		std::list<Node> *tokens_ = new std::list<Node>;
		Lexer            lex(buffer, *tokens_);
		PrintTokens(tokens_);
		delete tokens_;
	} catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
	}
}
