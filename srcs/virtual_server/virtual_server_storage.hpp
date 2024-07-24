#ifndef VIRTUAL_SERVER_STORAGE_HPP_
#define VIRTUAL_SERVER_STORAGE_HPP_

#include <list>
#include <map>

namespace server {

class VirtualServer;

// fdとVirtualServerを紐づけて全VirtualServerを保持・取得する
class VirtualServerStorage {
  public:
	typedef std::list<VirtualServer>             VirtualServerList;
	typedef std::map<int, const VirtualServer *> VirtualServerMapping;
	VirtualServerStorage();
	~VirtualServerStorage();
	VirtualServerStorage(const VirtualServerStorage &other);
	VirtualServerStorage &operator=(const VirtualServerStorage &other);
	// function
	void AddVirtualServer(const VirtualServer &virtual_server);

  private:
	// 全VirtualServerを保持
	VirtualServerList virtual_servers_;
	// VirtualServerと、VirtualServerが通信に使用している複数fdを紐づける
	VirtualServerMapping mapping_fd_to_virtual_servers_;
};

} // namespace server

#endif /* VIRTUAL_SERVER_STORAGE_HPP_ */
