#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <iostream>

enum Method {
	GET,
	POST,
	DELETE
};

struct LocationDir {
	std::string root_;
	std::string index_;
	std::string allowed_method_;
};

struct ServerDir {
	int         port_;
	std::string server_name_;
	LocationDir location_dir_;
};

#endif
