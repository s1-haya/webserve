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

// todo: PortListをstd::list<unsigned int>で作成？
typedef std::list<int>         PortList;
typedef std::list<LocationCon> LocationList;

struct ServerCon {
	PortList     port;
	std::string  server_name;
	LocationList location_con;
};

} // namespace context
} // namespace config

#endif
