#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include <list>
#include <string>

namespace config {
namespace context {

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

} // namespace context
} // namespace config

#endif
