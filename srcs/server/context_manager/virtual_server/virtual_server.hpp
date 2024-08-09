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
	typedef std::list<Location>     LocationList;
	typedef std::list<unsigned int> PortList;
	// default constructor: necessary for map's insert/[]
	VirtualServer();
	// todo: configもらう？
	VirtualServer(
		const std::string &server_name, const LocationList &locations, const PortList &ports
	);
	~VirtualServer();
	VirtualServer(const VirtualServer &other);
	VirtualServer &operator=(const VirtualServer &other);
	// getter
	const std::string  &GetServerName() const;
	const LocationList &GetLocations() const;
	const PortList     &GetPorts() const;

  private:
	// todo: add member(& operator=)
	std::string  server_name_;
	LocationList locations_; // todo
	PortList     ports_;
};

} // namespace server

#endif /* SERVER_CONTEXTMANAGER_VIRTUALSERVER_VIRTUALSERVER_HPP_ */
