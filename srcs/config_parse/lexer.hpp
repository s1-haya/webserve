#ifndef LEXER_HPP_
#define LEXER_HPP_
#include "node.hpp"
#include <list>
#include <vector>

namespace lexer {

class Lexer {
  private:
	std::list<Node>         &tokens_;
	std::vector<std::string> context_;
	std::vector<std::string> directive_;
	enum TokenType {
		DELIM,
		L_BRACKET,
		R_BRACKET,
		CONTEXT,
		DIRECTIVE,
		WORD
	};
	const std::string buffer_;

	Lexer();
	void InitDefinition();
	void LexBuffer();

	// Token Handler
	void      AddToken(char, TokenType);
	void      AddToken(const std::string &, TokenType);
	void      AddWordToken(std::string::const_iterator &);
	void      SkipComment(std::string::const_iterator &);
	TokenType SearchWordTokenType(std::string &);

	// Prohibit Copy
	Lexer(const Lexer &);
	Lexer &operator=(const Lexer &);

  public:
	Lexer(const std::string &, std::list<Node> &);
	~Lexer();

	// Definition
	static const char CR            = '\r';
	static const char LF            = '\n';
	static const char SEMICOLON_CHR = ';';
	static const char L_BRACKET_CHR = '{';
	static const char R_BRACKET_CHR = '}';
	static const char SHARP_CHR     = '#';
};

} // namespace lexer

#endif
