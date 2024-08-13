#include "lexer.hpp"
#include "directive_names.hpp"
#include <algorithm>
#include <iostream>

namespace config {
namespace lexer {
namespace {

bool IsSpace(char c) { // 必要か
	if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
		return true;
	}
	return false;
}

} // namespace

Lexer::Lexer(const std::string &buffer, std::list<node::Node> &tokens)
	: tokens_(tokens), buffer_(buffer) {
	InitDefinition();
	LexBuffer();
}

Lexer::~Lexer() {}

void Lexer::InitDefinition() {
	context_.push_back(SERVER);
	context_.push_back(LOCATION);

	directive_.push_back(SERVER_NAME);
	directive_.push_back(LISTEN);
	directive_.push_back(ALIAS);
	directive_.push_back(INDEX);
	directive_.push_back(AUTO_INDEX);
	directive_.push_back(ERROR_PAGE);
	directive_.push_back(CLIENT_MAX_BODY_SIZE);
	directive_.push_back(ALLOWED_METHODS);
	directive_.push_back(RETURN);
}

void Lexer::LexBuffer() {
	bool is_next_to_word = false;

	for (std::string::const_iterator it = buffer_.begin(); it != buffer_.end() && *it != '\0';
		 ++it) {
		if (IsSpace(*it)) {
			continue;
		}
		switch (*it) {
		case SEMICOLON_CHR:
			AddToken(SEMICOLON_CHR, node::DELIM);
			is_next_to_word = false;
			break;
		case L_BRACKET_CHR:
			AddToken(L_BRACKET_CHR, node::L_BRACKET);
			is_next_to_word = false;
			break;
		case R_BRACKET_CHR:
			AddToken(R_BRACKET_CHR, node::R_BRACKET);
			is_next_to_word = false;
			break;
		case SHARP_CHR:
			SkipComment(it);
			is_next_to_word = false;
			break;
		default:
			if (is_next_to_word) {
				AddWordToken(it);
			} else {
				AddContextDirectiveWordToken(it);
			}
			is_next_to_word = true;
			break;
		}
	}
}

// For char
void Lexer::AddToken(char symbol, node::TokenType token_type) {
	const std::string symbol_str(1, symbol);
	const node::Node  token = {symbol_str, token_type};
	tokens_.push_back(token);
}

// For string
void Lexer::AddToken(const std::string &symbol, node::TokenType token_type) {
	const node::Node token = {symbol, token_type};
	tokens_.push_back(token);
}

/*LexBuffer()内でfor文で++itしているため、最後の文字から次の処理をさせるために各関数の最後に--itがついています
ex. server -> rから次のfor文に*/

void Lexer::AddWordToken(std::string::const_iterator &it) {
	std::string new_str;
	while (it != buffer_.end() && !IsSpace(*it) && *it != SEMICOLON_CHR) {
		new_str += *it;
		++it;
	}
	AddToken(new_str, node::WORD);
	--it;
}

void Lexer::AddContextDirectiveWordToken(std::string::const_iterator &it) {
	std::string new_str;
	while (it != buffer_.end() && !IsSpace(*it) && *it != SEMICOLON_CHR) {
		new_str += *it;
		++it;
	}
	AddToken(new_str, SearchWordTokenType(new_str)); // 予約語の検索
	--it;
}

void Lexer::SkipComment(std::string::const_iterator &it) {
	while (it != buffer_.end() && *it != '\n') {
		++it;
	}
	--it;
}

node::TokenType Lexer::SearchWordTokenType(std::string &word) {
	if (std::find(context_.begin(), context_.end(), word) != context_.end()) {
		return node::CONTEXT;
	} else if (std::find(directive_.begin(), directive_.end(), word) != directive_.end()) {
		return node::DIRECTIVE;
	}
	return node::WORD;
}

} // namespace lexer
} // namespace config
