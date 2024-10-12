#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include <list>
#include <string>
#include <utility>

namespace config {
namespace context {

struct LocationCon {
	std::string                          request_uri;
	std::string                          alias;
	std::string                          index;
	bool                                 autoindex;
	std::list<std::string>               allowed_methods;
	std::pair<unsigned int, std::string> redirect; // cannot use return
	std::string                          cgi_extension;
	std::string                          upload_directory;
	LocationCon() : autoindex(false) {}
};

typedef std::list<LocationCon>               LocationList;
typedef std::pair<std::string, unsigned int> HostPortPair;
typedef std::list<HostPortPair>              HostPortList;

struct ServerCon {
	HostPortList                         host_ports;
	std::list<std::string>               server_names;
	LocationList                         location_con;
	std::size_t                          client_max_body_size;
	std::pair<unsigned int, std::string> error_page;
	// default value for client_max_body_size is 1MB
	ServerCon() : client_max_body_size(1024 * 1024) {}
};

} // namespace context
} // namespace config

#endif
