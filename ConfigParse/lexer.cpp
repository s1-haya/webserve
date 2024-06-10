#include "lexer.hpp"
#include <iostream>

namespace {
	bool IsSpace(char c) {
		if (c == ' ' || c == '\t' || c == CR || c == LF)
			return true;
		return false;
	}
} // namespace

void Lexer::AddToken(std::string symbol, int token_type) {
	Node *token = new Node(symbol, token_type);
	tokens_->push_back(token);
}

void Lexer::AddTokenIncrement(
	std::string token, int token_type, std::string::const_iterator &it
) {
	AddToken(token, token_type);
	it += std::strlen(token.c_str()) - 1;
}

void Lexer::AddTokenElem(
	std::string token, int token_type, std::string::const_iterator &it
) {
	std::string new_str = "";
	AddToken(token, token_type);
	it += std::strlen(token.c_str());
	while (IsSpace(*it))
		++it;
	while (*it != DELIM_CHR && *it != '\n') {
		new_str += *it;
		++it;
	}
	if (*it == '\n')
		throw std::runtime_error("no delim");
	AddToken(new_str, 10);
	AddToken(";", DELIM);
}

Lexer::Lexer(const std::string &buffer) : buffer_(buffer) {
	tokens_                   = new std::list<Node *>;
	bool        need_space    = false;
	bool        need_delim    = false;
	bool        sharp_comment = false;
	std::string new_str;

	for (std::string::const_iterator it = buffer.begin(); it != buffer.end(); ++it) {
		while (IsSpace(*it)) {
			++it;
			need_space = false;
		}
		if (!need_space && !sharp_comment) {
			if (std::strncmp(
					&(*it), SERVER_NAME_STR, std::strlen(SERVER_NAME_STR)
				) == 0)
				AddTokenElem(SERVER_NAME_STR, SERVER_NAME, it);
			else if (std::strncmp(&(*it), SERVER_STR, std::strlen(SERVER_STR)) == 0)
				AddTokenIncrement(SERVER_STR, SERVER, it);
			else if (std::strncmp(
						 &(*it), L_BRACKET_STR, std::strlen(L_BRACKET_STR)
					 ) == 0)
				AddTokenIncrement(L_BRACKET_STR, L_BRACKET, it);
			else if (std::strncmp(
						 &(*it), R_BRACKET_STR, std::strlen(R_BRACKET_STR)
					 ) == 0)
				AddTokenIncrement(R_BRACKET_STR, R_BRACKET, it);
			else if (std::strncmp(&(*it), LOCATION_STR, std::strlen(LOCATION_STR)) ==
					 0)
				AddTokenIncrement(LOCATION_STR, LOCATION, it);
			else if (std::strncmp(&(*it), LISTEN_STR, std::strlen(LISTEN_STR)) == 0)
				AddTokenElem(LISTEN_STR, LISTEN, it);
			else if (std::strncmp(&(*it), ROOT_STR, std::strlen(ROOT_STR)) == 0)
				AddTokenElem(ROOT_STR, ROOT, it);
			else if (std::strncmp(&(*it), INDEX_STR, std::strlen(INDEX_STR)) == 0)
				AddTokenElem(INDEX_STR, INDEX, it);
			else if (std::strncmp(&(*it), SLASH_STR, std::strlen(SLASH_STR)) == 0)
				AddTokenIncrement(SLASH_STR, SLASH, it);
			else if (std::strncmp(&(*it), SHARP_STR, std::strlen(SHARP_STR)) == 0) {
				while (*it != '\n')
					++it;
			} else
				throw std::runtime_error("error");
		}
	}
}

Lexer::~Lexer() {}

void Lexer::PrintTokens() {
	std::list<Node *> tmp = *tokens_;
	while (!tmp.empty()) {
		Node *node = tmp.front();
		tmp.pop_front();
		std::cout << node->GetToken() << std::endl;
	}
}

#include <fstream>
#include <sstream>

int main() {
	std::ifstream     conf("config_samp");
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
