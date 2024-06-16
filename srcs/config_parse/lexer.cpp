#include "lexer.hpp"
#include "../utils/isspace.hpp"
#include <iostream>

void Lexer::AddToken(const char symbol, int token_type) {
	std::string symbol_str(1, symbol);
	Node        token(symbol_str, token_type);
	tokens_.push_back(token);
}

void Lexer::AddToken(const std::string &symbol, int token_type) {
	Node token(symbol, token_type);
	tokens_.push_back(token);
}

void Lexer::AddWordToken(std::string::const_iterator &it) {
	std::string new_str = "";
	while (!utils::IsSpace(*it) && *it != DELIM_CHR) {
		new_str += *it;
		++it;
	}
	AddToken(new_str, WORD);
	AddToken(DELIM_CHR, DELIM);
}

void Lexer::SkipComment(std::string::const_iterator &it) {
	while (*it != '\n')
		++it;
}

Lexer::Lexer(const std::string &buffer, std::list<Node> &tokens_)
	: tokens_(tokens_), buffer_(buffer) {
	bool need_space = false;
	// bool        need_delim    = false; 使うかわからない
	bool        sharp_comment = false;
	std::string new_str;

	for (std::string::const_iterator it = buffer_.begin(); it != buffer_.end();
		 ++it) {
		while (utils::IsSpace(*it)) {
			++it;
			need_space = false;
		}
		if (!need_space && !sharp_comment) {
			switch (*it) {
			case DELIM_CHR:
				AddToken(DELIM_CHR, DELIM);
				break;
			case L_BRACKET_CHR:
				AddToken(L_BRACKET_CHR, L_BRACKET);
				break;
			case R_BRACKET_CHR:
				AddToken(R_BRACKET_CHR, R_BRACKET);
				break;
			case SHARP_CHR:
				SkipComment(it);
				break;
			default:
				AddWordToken(it);
				break;
			}
		}
	}
}

Lexer::~Lexer() {}

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
