#ifndef SERVER_CONTEXTMANAGER_VIRTUALSERVER_VIRTUALSERVERSTORAGE_HPP_
#define SERVER_CONTEXTMANAGER_VIRTUALSERVER_VIRTUALSERVERSTORAGE_HPP_

#include <list>
#include <map>

namespace server {

class VirtualServer;

// fdとVirtualServerを紐づけて全VirtualServerを保持・取得する
class VirtualServerStorage {
  public:
	typedef std::list<VirtualServer> VirtualServerList;

	typedef std::list<const VirtualServer *>     VirtualServerAddrList;
	typedef std::map<int, VirtualServerAddrList> VirtualServerAddrListMap;

	VirtualServerStorage();
	~VirtualServerStorage();
	VirtualServerStorage(const VirtualServerStorage &other);
	VirtualServerStorage &operator=(const VirtualServerStorage &other);
	// functions
	void AddVirtualServer(const VirtualServer &virtual_server);
	void AddMapping(int server_fd, const VirtualServer *virtual_server);
	// getter
	const VirtualServerAddrList &GetVirtualServerAddrList(int server_fd) const;
	const VirtualServerList     &GetAllVirtualServerList() const;

  private:
	// 全VirtualServerを保持
	VirtualServerList virtual_servers_;
	// server_fd(host:port)が属するVirtualServerAddrのlistを保持
	VirtualServerAddrListMap virtual_server_addr_list_map_;
};

} // namespace server

#endif /* SERVER_CONTEXTMANAGER_VIRTUALSERVER_VIRTUALSERVERSTORAGE_HPP_ */
