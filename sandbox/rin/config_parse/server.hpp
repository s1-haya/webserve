#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <iostream>

enum Method {
	GET,
	POST,
	DELETE
};

enum TokenType { /*仮置き*/
	DELIM,
	L_BRACKET,
	R_BRACKET,
	SHARP,
	CONTEXT,
	DIRECTIVE,
	WORD
};

struct LocationCon {
	std::string root_;
	std::string index_;
	std::string allowed_method_;
};

struct ServerCon {
	int         port_;
	std::string server_name_;
	LocationCon location_dir_;
};

#endif
