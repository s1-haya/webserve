#ifndef EPOLL_HPP_
#define EPOLL_HPP_

#include "debug.hpp" // todo: tmp
#include "event.hpp"
#include <sys/epoll.h> // epoll

class Epoll {
  public:
	Epoll();
	~Epoll();
	void AddNewConnection(int socket_fd, EventType type);
	void DeleteConnection(int socket_fd);
	int  CreateReadyList();
	// getter
	Event GetEvent(std::size_t index) const;

  private:
	// prohibit copy
	Epoll(const Epoll &other);
	Epoll &operator=(const Epoll &other);
	// const
	static const int          SYSTEM_ERROR = -1;
	static const unsigned int MAX_EVENTS   = 10;
	// variables
	int                epoll_fd_;
	struct epoll_event evlist_[MAX_EVENTS];
};

#endif /* EPOLL_HPP_ */
