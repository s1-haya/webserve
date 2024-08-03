#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include <list>
#include <string>

namespace config {
namespace context {

struct LocationCon {
	std::string                          location;
	std::string                          root;
	std::string                          index;
	bool                                 autoindex;
	std::pair<unsigned int, std::string> error_page;
	std::list<std::string>               allowed_methods;
};

// todo: PortListをstd::list<unsigned int>で作成？
typedef std::list<int>         PortList;
typedef std::list<LocationCon> LocationList;

struct ServerCon {
	PortList               port;
	std::list<std::string> server_names;
	LocationList           location_con;
	std::size_t            client_max_body_size;
};

} // namespace context
} // namespace config

#endif
