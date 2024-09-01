#ifndef SERVER_CONTEXTMANAGER_VIRTUALSERVER_VIRTUALSERVER_HPP_
#define SERVER_CONTEXTMANAGER_VIRTUALSERVER_VIRTUALSERVER_HPP_

#include <list>
#include <string>

namespace server {

// todo: in VirtualServer class?
struct Location {
	std::string location;
	std::string root;
	std::string index;
	std::string allowed_method;
};

// virtual serverとして必要な情報を保持・取得する
class VirtualServer {
  public:
	typedef std::list<Location>                  LocationList;
	typedef std::pair<std::string, unsigned int> HostPortPair;
	typedef std::list<HostPortPair>              HostPortList;

	// default constructor: necessary for map's insert/[]
	VirtualServer();
	// todo: configもらう？
	VirtualServer(
		const std::string  &server_name,
		const LocationList &locations,
		const HostPortList &host_port_list
	);
	~VirtualServer();
	VirtualServer(const VirtualServer &other);
	VirtualServer &operator=(const VirtualServer &other);
	// getter
	const std::string  &GetServerName() const;
	const LocationList &GetLocations() const;
	const HostPortList &GetHostPortList() const;

  private:
	// todo: add member(& operator=)
	std::string  server_name_;
	LocationList locations_; // todo
	HostPortList host_port_list_;
};

} // namespace server

#endif /* SERVER_CONTEXTMANAGER_VIRTUALSERVER_VIRTUALSERVER_HPP_ */
