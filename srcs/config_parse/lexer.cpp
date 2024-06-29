#include "lexer.hpp"
#include "../utils/isspace.hpp"
#include <algorithm>
#include <iostream>

Lexer::Lexer(const std::string &buffer, std::list<Node> &tokens_)
	: tokens_(tokens_), buffer_(buffer) {
	std::string new_str;
	InitDefinition();
	LexBuffer();
}

Lexer::~Lexer() {}

void Lexer::InitDefinition() {
	context_.push_back("server");
	context_.push_back("location");

	directive_.push_back("server_name");
	directive_.push_back("listen");
	directive_.push_back("rewrite");
	directive_.push_back("root");
	directive_.push_back("index");
}

void Lexer::LexBuffer() {
	for (std::string::const_iterator it = buffer_.begin(); it != buffer_.end(); ++it) {
		while (utils::IsSpace(*it)) {
			++it;
		}
		switch (*it) {
		case SEMICOLON_CHR:
			AddToken(SEMICOLON_CHR, DELIM);
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

// For char
void Lexer::AddToken(const char symbol, int token_type) {
	std::string symbol_str(1, symbol);
	Node        token(symbol_str, token_type);
	tokens_.push_back(token);
}

// For string
void Lexer::AddToken(const std::string &symbol, int token_type) {
	Node token(symbol, token_type);
	tokens_.push_back(token);
}

void Lexer::AddWordToken(std::string::const_iterator &it) {
	std::string new_str = "";
	while (!utils::IsSpace(*it) && *it != SEMICOLON_CHR) {
		new_str += *it;
		++it;
	}
	if (SearchWordTokenType(new_str) == CONTEXT) // 予約語の検索
		AddToken(new_str, CONTEXT);
	else if (SearchWordTokenType(new_str) == DIRECTIVE) // ..
		AddToken(new_str, DIRECTIVE);
	else
		AddToken(new_str, WORD);
	it -= 1; // loopでWordの次の文字から処理
}

void Lexer::SkipComment(std::string::const_iterator &it) {
	while (*it != '\n')
		++it;
}

Lexer::TokenType Lexer::SearchWordTokenType(std::string &word) {
	if (std::find(context_.begin(), context_.end(), word) != context_.end())
		return CONTEXT;
	else if (std::find(directive_.begin(), directive_.end(), word) != directive_.end())
		return DIRECTIVE;
	return WORD;
}
