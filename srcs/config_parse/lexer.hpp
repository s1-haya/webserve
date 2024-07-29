#ifndef LEXER_HPP_
#define LEXER_HPP_
#include "node.hpp"
#include <list>
#include <vector>

namespace config {
namespace lexer {

class Lexer {
  private:
	std::list<node::Node>   &tokens_;
	std::vector<std::string> context_;
	std::vector<std::string> directive_;
	const std::string        buffer_;

	Lexer();
	void InitDefinition();
	void LexBuffer();

	// Token Handler
	void            AddToken(char, node::TokenType);
	void            AddToken(const std::string &, node::TokenType);
	void            AddWordToken(std::string::const_iterator &);
	void            AddContextDirectiveWordToken(std::string::const_iterator &);
	void            SkipComment(std::string::const_iterator &);
	node::TokenType SearchWordTokenType(std::string &);

	// Prohibit Copy
	Lexer(const Lexer &);
	Lexer &operator=(const Lexer &);

  public:
	Lexer(const std::string &, std::list<node::Node> &);
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
} // namespace config

#endif
