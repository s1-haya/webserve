#ifndef LEXER_HPP_
#define LEXER_HPP_
#include <list>

#include "node.hpp"

class Lexer {
  private:
	std::list<Node *> *tokens_;
	void               AddToken(std::string symbol, int token_type);
	void               AddTokenIncrement(
					  std::string token, int token_type, std::string::const_iterator &it
				  );
	void
	AddTokenElem(std::string token, int token_type, std::string::const_iterator &it);
	enum {
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
	};

  public:
	Lexer(const std::string &);
	~Lexer();
	void PrintTokens();
};

/*--------------------------------------*/
#define LF              '\n'
#define CR              '\r'

#define SERVER_STR      "server"
#define SERVER_NAME_STR "server_name"
#define LOCATION_STR    "location"
#define L_BRACKET_STR   "{"
#define R_BRACKET_STR   "}"
#define LISTEN_STR      "listen"
#define DELIM_CHR       ';'
#define ROOT_STR        "root"
#define INDEX_STR       "index"
#define SLASH_STR       "/"
/*--------------------------------------*/

#endif