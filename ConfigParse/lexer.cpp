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

void Lexer::AddToken(const std::string& symbol, int token_type) {
	Node token = Node(symbol, token_type);
	tokens_.push_back(token);
}

void Lexer::AddTokenIncrement(
	const std::string& token, int token_type, std::string::const_iterator &it
) {
	AddToken(token, token_type);
	it += token.size() - 1;
}

void Lexer::AddTokenElem(
	const std::string& token, int token_type, std::string::const_iterator &it
) {
	std::string new_str = "";
	AddToken(token, token_type);
	it += token.size();
	while (IsSpace(*it))
		++it;
	while (*it != DELIM_CHR && *it != '\n') {
		new_str += *it;
		++it;
	}
	if (*it == '\n')
		throw std::runtime_error("no delim");
	AddToken(new_str, STRING);
	AddToken(";", DELIM);
}

Lexer::Lexer(const std::string &buffer, std::list<Node>& tokens_) : buffer_(buffer), tokens_(tokens_) {
	bool        need_space    = false;
	bool        need_delim    = false;
	bool        sharp_comment = false;
	std::string new_str;

	for (std::string::const_iterator it = buffer_.begin(); it != buffer_.end(); ++it) {
		while (IsSpace(*it)) {
			++it;
			need_space = false;
		}
		if (!need_space && !sharp_comment) {
			if (std::equal(Lexer::SERVER_NAME_STR.begin(), Lexer::SERVER_NAME_STR.end(), it))
				AddTokenElem(Lexer::SERVER_NAME_STR, SERVER_NAME, it);
			else if (std::equal(Lexer::SERVER_STR.begin(), Lexer::SERVER_STR.end(), it))
				AddTokenIncrement(Lexer::SERVER_STR, SERVER, it);
			else if (std::equal(Lexer::L_BRACKET_STR.begin(), Lexer::L_BRACKET_STR.end(), it))
				AddTokenIncrement(Lexer::L_BRACKET_STR, L_BRACKET, it);
			else if (std::equal(Lexer::R_BRACKET_STR.begin(), Lexer::R_BRACKET_STR.end(), it))
				AddTokenIncrement(Lexer::R_BRACKET_STR, R_BRACKET, it);
			else if (std::equal(Lexer::LOCATION_STR.begin(), Lexer::LOCATION_STR.end(), it))
				AddTokenIncrement(Lexer::LOCATION_STR, LOCATION, it);
			else if (std::equal(Lexer::LISTEN_STR.begin(), Lexer::LISTEN_STR.end(), it))
				AddTokenElem(Lexer::LISTEN_STR, LISTEN, it);
			else if (std::equal(Lexer::ROOT_STR.begin(), Lexer::ROOT_STR.end(), it))
				AddTokenElem(Lexer::ROOT_STR, ROOT, it);
			else if (std::equal(Lexer::INDEX_STR.begin(), Lexer::INDEX_STR.end(), it))
				AddTokenElem(Lexer::INDEX_STR, INDEX, it);
			else if (std::equal(Lexer::SLASH_STR.begin(), Lexer::SLASH_STR.end(), it))
				AddTokenIncrement(Lexer::SLASH_STR, SLASH, it);
			else if (std::equal(Lexer::SHARP_STR.begin(), Lexer::SHARP_STR.end(), it)) {
				while (*it != Lexer::LF)
					++it;
			} else
				throw std::runtime_error("Unknown token encountered: " + std::string(it, buffer_.end()));
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
			std::cout << node.token_ << std::endl;
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
		Lexer lex(buffer, *tokens_);
		PrintTokens(tokens_);
		delete tokens_;
	} catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
	}
}
