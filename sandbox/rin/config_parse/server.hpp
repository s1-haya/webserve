#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <iostream>
#include <list>

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
	std::string location_; // 変える??
	std::string root_;
	std::string index_;
	std::string allowed_method_;
};

struct ServerCon {
	int                    port_;
	std::string            server_name_;
	std::list<LocationCon> location_con_;
};

#endif
