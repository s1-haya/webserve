#ifndef VIRTUAL_SERVER_HPP_
#define VIRTUAL_SERVER_HPP_

#include <list>
#include <string>

namespace server {

// virtual serverとして必要な情報を保持・取得する
class VirtualServer {
  public:
	typedef std::list<std::string>  LocationList;
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

#endif /* VIRTUAL_SERVER_HPP_ */
