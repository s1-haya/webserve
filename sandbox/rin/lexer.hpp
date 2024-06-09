#ifndef LEXER_HPP_
#define LEXER_HPP_
#include <queue>

#include "node.hpp"

class Lexer {
  private:
	std::queue<Node *> *tokens_;
	void                AddToken(std::string symbol, int token_type);

  public:
	Lexer(std::string &);
	~Lexer();
	void PrintTokens();
};

/*--------------------------------------*/
#define LF '\n'
#define CR '\r'
static bool IsSpace(char c) {
	if (c == ' ' || c == '\t' || c == CR || c == LF)
		return true;
	return false;
}

enum {
	SERVER,
	SERVER_NAME,
	LOCATION,
	DELIM,
	SHARP,
	L_BRACKET,
	R_BRACKET,
	LISTEN,
};

#define SERVER_STR      "server"
#define SERVER_NAME_STR "server_name"
#define LOCATION_STR    "location"
#define L_BRACKET_STR   "{"
#define R_BRACKET_STR   "}"
#define LISTEN_STR      "listen"
#define DELIM_CHR       ';'
/*--------------------------------------*/

#endif