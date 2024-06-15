#include "lexer.hpp"
#include <iostream>

namespace {
	bool IsSpace(char c) {
		if (c == ' ' || c == '\t' || c == Lexer::CR || c == Lexer::LF)
			return true;
		return false;
	}
} // namespace

const std::string Lexer::SERVER_STR = "server";
const std::string Lexer::SERVER_NAME_STR = "server_name";
const std::string Lexer::LOCATION_STR = "location";
const std::string Lexer::L_BRACKET_STR = "{";
const std::string Lexer::R_BRACKET_STR = "}";
const std::string Lexer::LISTEN_STR = "listen";
const std::string Lexer::ROOT_STR = "root";
const std::string Lexer::INDEX_STR = "index";
const std::string Lexer::SLASH_STR = "/";
const std::string Lexer::SHARP_STR = "#";

void Lexer::AddToken(std::string symbol, int token_type) {
	Node token = Node(symbol, token_type);
	tokens_.push_back(token);
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

Lexer::Lexer(const std::string &buffer, std::list<Node>* tokens_) : buffer_(buffer), tokens_(*tokens_) {
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
					&(*it), Lexer::SERVER_NAME_STR.c_str(), std::strlen(Lexer::SERVER_NAME_STR.c_str())
				) == 0)
				AddTokenElem(Lexer::SERVER_NAME_STR.c_str(), SERVER_NAME, it);
			else if (std::strncmp(&(*it), Lexer::SERVER_STR.c_str(), std::strlen(Lexer::SERVER_STR.c_str())) == 0)
				AddTokenIncrement(Lexer::SERVER_STR.c_str(), SERVER, it);
			else if (std::strncmp(
						 &(*it), Lexer::L_BRACKET_STR.c_str(), std::strlen(Lexer::L_BRACKET_STR.c_str())
					 ) == 0)
				AddTokenIncrement(Lexer::L_BRACKET_STR.c_str(), L_BRACKET, it);
			else if (std::strncmp(
						 &(*it), Lexer::R_BRACKET_STR.c_str(), std::strlen(Lexer::R_BRACKET_STR.c_str())
					 ) == 0)
				AddTokenIncrement(Lexer::R_BRACKET_STR.c_str(), R_BRACKET, it);
			else if (std::strncmp(&(*it), Lexer::LOCATION_STR.c_str(), std::strlen(Lexer::LOCATION_STR.c_str())) ==
					 0)
				AddTokenIncrement(Lexer::LOCATION_STR.c_str(), LOCATION, it);
			else if (std::strncmp(&(*it), Lexer::LISTEN_STR.c_str(), std::strlen(Lexer::LISTEN_STR.c_str())) == 0)
				AddTokenElem(Lexer::LISTEN_STR.c_str(), LISTEN, it);
			else if (std::strncmp(&(*it), Lexer::ROOT_STR.c_str(), std::strlen(Lexer::ROOT_STR.c_str())) == 0)
				AddTokenElem(Lexer::ROOT_STR.c_str(), ROOT, it);
			else if (std::strncmp(&(*it), Lexer::INDEX_STR.c_str(), std::strlen(Lexer::INDEX_STR.c_str())) == 0)
				AddTokenElem(Lexer::INDEX_STR.c_str(), INDEX, it);
			else if (std::strncmp(&(*it), Lexer::SLASH_STR.c_str(), std::strlen(Lexer::SLASH_STR.c_str())) == 0)
				AddTokenIncrement(Lexer::SLASH_STR.c_str(), SLASH, it);
			else if (std::strncmp(&(*it), Lexer::SHARP_STR.c_str(), std::strlen(Lexer::SHARP_STR.c_str())) == 0) {
				while (*it != Lexer::LF)
					++it;
			} else
				throw std::runtime_error("error");
		}
	}
}

Lexer::~Lexer() {}

namespace
{
	void PrintTokens(std::list<Node>* tokens_) { /*デバッグ用*/
		std::list<Node> tmp = *tokens_;
		while (!tmp.empty()) {
			Node node = tmp.front();
			tmp.pop_front();
			std::cout << node.GetToken() << std::endl;
		}
	}
} // namespace

#include <fstream>
#include <sstream>

int main() {
	std::ifstream     conf("config_samp");
	std::stringstream ss;
	ss << conf.rdbuf();
	std::string buffer = ss.str();
	try {
		std::list<Node>* tokens_ = new std::list<Node>;
		Lexer lex(buffer, tokens_);
		PrintTokens(tokens_);
		delete tokens_;
	} catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
	}
}
