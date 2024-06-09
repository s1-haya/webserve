#include "lexer.hpp"
#include <iostream>

void Lexer::AddToken(std::string symbol, int token_type) {
	Node *token = new Node(symbol, token_type);
	tokens_->push(token);
}

Lexer::Lexer(std::string &buffer) {
	tokens_                   = new std::queue<Node *>;
	bool        need_space    = false;
	bool        need_delim    = false;
	bool        sharp_comment = false;
	std::string new_str;

	for (std::string::const_iterator it = buffer.begin(); it != buffer.end(); ++it) {
		while (IsSpace(*it)) {
			++it;
			need_space = false;
		}
		printf("%c\n", *it);
		if (!need_space) {
			if (std::strncmp(
					&(*it), SERVER_NAME_STR, std::strlen(SERVER_NAME_STR)
				) == 0) {
				AddToken(SERVER_NAME_STR, SERVER_NAME);
				it += std::strlen(SERVER_NAME_STR);
				while (IsSpace(*it))
					++it;
				new_str = "";
				while (*it != DELIM_CHR) {
					new_str += *it;
					++it;
				}
				AddToken(new_str, 10);
				std::cout << new_str << std::endl;
			} else if (std::strncmp(&(*it), SERVER_STR, std::strlen(SERVER_STR)) ==
					   0) {
				AddToken(SERVER_STR, SERVER);
				it += std::strlen(SERVER_STR);
			} else if (std::strncmp(
						   &(*it), L_BRACKET_STR, std::strlen(L_BRACKET_STR)
					   ) == 0) {
				AddToken(L_BRACKET_STR, L_BRACKET);
				it += std::strlen(L_BRACKET_STR);
			} else if (std::strncmp(
						   &(*it), R_BRACKET_STR, std::strlen(R_BRACKET_STR)
					   ) == 0) {
				AddToken(R_BRACKET_STR, R_BRACKET);
				it += std::strlen(R_BRACKET_STR);
			}
			// sharp_comment = true;
			else if (std::strncmp(&(*it), LISTEN_STR, std::strlen(LISTEN_STR)) ==
					 0) {
				AddToken(LISTEN_STR, LISTEN);
				it += std::strlen(LISTEN_STR);
				while (IsSpace(*it))
					++it;
				new_str = "";
				while (*it != DELIM_CHR) {
					new_str += *it;
					++it;
				}
				AddToken(new_str, 10);
			} else
				throw std::runtime_error("error"); /*適当*/
		}
	}
}

Lexer::~Lexer() {}

void Lexer::PrintTokens() {
	std::queue<Node *> tmp = *tokens_;
	while (!tmp.empty()) {
		Node *node = tmp.front();
		tmp.pop();
		std::cout << node->GetToken() << std::endl;
	}
}

#include <fstream>
#include <sstream>

int main() {
	std::ifstream     conf("config");
	std::stringstream ss;
	ss << conf.rdbuf();
	std::string buffer = ss.str();
	try {
		Lexer lex(buffer);
		lex.PrintTokens();
	} catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
	}
}
