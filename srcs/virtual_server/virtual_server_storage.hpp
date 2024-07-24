#ifndef VIRTUAL_SERVER_STORAGE_HPP_
#define VIRTUAL_SERVER_STORAGE_HPP_

#include <list>

namespace server {

class VirtualServer;

// fdとVirtualServerを紐づけて全VirtualServerを保持・取得する
class VirtualServerStorage {
  public:
	typedef std::list<VirtualServer> VirtualServerList;
	VirtualServerStorage();
	~VirtualServerStorage();
	VirtualServerStorage(const VirtualServerStorage &other);
	VirtualServerStorage &operator=(const VirtualServerStorage &other);
	// function
	void AddVirtualServer(const VirtualServer &virtual_server);

  private:
	// 全VirtualServerを保持
	VirtualServerList virtual_servers_;
};

} // namespace server

#endif /* VIRTUAL_SERVER_STORAGE_HPP_ */
