#ifndef SERVER_CONTEXTMANAGER_VIRTUALSERVER_VIRTUALSERVER_HPP_
#define SERVER_CONTEXTMANAGER_VIRTUALSERVER_VIRTUALSERVER_HPP_

#include <cstddef> // size_t
#include <list>
#include <string>

namespace server {

// todo: in VirtualServer class?
struct Location {
	typedef std::list<std::string>               AllowedMethodList;
	typedef std::pair<unsigned int, std::string> Redirect;

	Location() : autoindex(false), redirect(std::make_pair(0, "")) {}

	std::string       request_uri;
	std::string       alias;
	std::string       index;
	bool              autoindex;
	AllowedMethodList allowed_methods;
	Redirect          redirect;
	std::string       cgi_extension;
	std::string       upload_directory;
};

// virtual serverとして必要な情報を保持・取得する
class VirtualServer {
  public:
	typedef std::list<Location>                  LocationList;
	typedef std::pair<std::string, unsigned int> HostPortPair;
	typedef std::list<HostPortPair>              HostPortList;
	typedef std::pair<unsigned int, std::string> ErrorPage;

	// default constructor: necessary for map's insert/[]
	VirtualServer();
	VirtualServer(
		const std::string  &server_name,
		const LocationList &locations,
		const HostPortList &host_ports,
		std::size_t         client_max_body_size,
		const ErrorPage    &error_page
	);
	~VirtualServer();
	VirtualServer(const VirtualServer &other);
	VirtualServer &operator=(const VirtualServer &other);
	// getter
	const std::string  &GetServerName() const;
	const LocationList &GetLocations() const;
	const HostPortList &GetHostPortList() const;
	std::size_t         GetClientMaxBodySize() const;
	const ErrorPage    &GetErrorPage() const;

  private:
	static const std::size_t DEFAULT_CLIENT_MAX_BODY_SIZE = 1024;
	// variables
	std::string  server_name_;
	LocationList locations_;
	HostPortList host_ports_;
	std::size_t  client_max_body_size_;
	ErrorPage    error_page_;
};

} // namespace server

#endif /* SERVER_CONTEXTMANAGER_VIRTUALSERVER_VIRTUALSERVER_HPP_ */
