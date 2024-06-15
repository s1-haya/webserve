#ifndef LEXER_HPP_
#define LEXER_HPP_
#include <list>

#include "node.hpp"

class Lexer {
  private:
	std::list<Node> &tokens_;
	Lexer();
	void               AddToken(const std::string& symbol, int token_type);
	void               AddTokenIncrement(
					  const std::string& token, int token_type, std::string::const_iterator &it
				  );
	void
	AddTokenElem(const std::string& token, int token_type, std::string::const_iterator &it);
	enum TokenType {
		SERVER,
		SERVER_NAME,
		LOCATION,
		DELIM,
		SHARP,
		L_BRACKET,
		R_BRACKET,
		LISTEN,
		ROOT,
		INDEX,
		SLASH,
		STRING,
	};
	const std::string buffer_;

  public:
	Lexer(const std::string &, std::list<Node>&);
	~Lexer();
	void PrintTokens();
	static const char CR = '\r';
	static const char LF = '\n';
	static const char DELIM_CHR = ';';
	static const std::string SERVER_STR;
	static const std::string SERVER_NAME_STR;
	static const std::string LOCATION_STR;
	static const std::string L_BRACKET_STR;
	static const std::string R_BRACKET_STR;
	static const std::string LISTEN_STR;
	static const std::string ROOT_STR;
	static const std::string INDEX_STR;
	static const std::string SLASH_STR;
	static const std::string SHARP_STR;
};

#endif