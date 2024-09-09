#ifndef SERVER_CONTEXTMANAGER_VIRTUALSERVERSTORAGE_HPP_
#define SERVER_CONTEXTMANAGER_VIRTUALSERVERSTORAGE_HPP_

#include "virtual_server.hpp"
#include <list>
#include <map>

namespace server {

// fdとVirtualServerを紐づけて全VirtualServerを保持・取得する
class VirtualServerStorage {
  public:
	typedef std::list<VirtualServer> VirtualServerList;

	typedef VirtualServer::HostPortPair                   HostPortPair;
	typedef std::list<const VirtualServer *>              VirtualServerAddrList;
	typedef std::map<HostPortPair, VirtualServerAddrList> VirtualServerAddrListMap;

	VirtualServerStorage();
	~VirtualServerStorage();
	VirtualServerStorage(const VirtualServerStorage &other);
	VirtualServerStorage &operator=(const VirtualServerStorage &other);
	// functions
	void AddVirtualServer(const VirtualServer &virtual_server);
	void InitHostPortPair(const HostPortPair &host_port);
	void AddMapping(const HostPortPair &host_port, const VirtualServer *virtual_server);
	// getter
	const VirtualServerAddrList &GetVirtualServerAddrList(const HostPortPair &host_port) const;
	const VirtualServerList     &GetAllVirtualServerList() const;

  private:
	// 全VirtualServerを保持
	VirtualServerList virtual_servers_;
	// host:portが属するVirtualServerAddrのlistを保持
	VirtualServerAddrListMap virtual_server_addr_list_map_;
};

} // namespace server

#endif /* SERVER_CONTEXTMANAGER_VIRTUALSERVERSTORAGE_HPP_ */
