#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include <list>
#include <string>

namespace config {
namespace context {

struct LocationCon {
	std::string                          request_uri;
	std::string                          alias;
	std::string                          index;
	bool                                 autoindex;
	std::list<std::string>               allowed_methods;
	std::pair<unsigned int, std::string> redirect;         // cannot use return
	std::string                          cgi_extension;    // not implement
	std::string                          upload_directory; // not implement
	LocationCon() : autoindex(false) {}
};

typedef std::list<unsigned int> PortList;
typedef std::list<LocationCon>  LocationList;

struct ServerCon {
	std::string                          host; // not implement
	PortList                             port;
	std::list<std::string>               server_names;
	LocationList                         location_con;
	std::size_t                          client_max_body_size;
	std::pair<unsigned int, std::string> error_page;
	ServerCon() : client_max_body_size(1024) {}
};

} // namespace context
} // namespace config

#endif
