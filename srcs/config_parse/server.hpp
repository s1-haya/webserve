#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <list>
#include <string>

struct LocationCon {
	std::string location;
	std::string root;
	std::string index;
	std::string allowed_method;
};

struct ServerCon {
	std::list<int>         port;
	std::string            server_name;
	std::list<LocationCon> location_con;
};

#endif
