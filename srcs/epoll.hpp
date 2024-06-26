#ifndef EPOLL_HPP_
#define EPOLL_HPP_

#include "event.hpp"
#include <cstddef>     // size_t
#include <sys/epoll.h> // epoll

namespace epoll {

class Epoll {
  public:
	Epoll();
	~Epoll();
	void AddNewConnection(int socket_fd, event::Type type);
	void DeleteConnection(int socket_fd);
	void UpdateEventType(const event::Event &event, event::Type new_type);
	int  CreateReadyList();
	// getter
	event::Event GetEvent(std::size_t index) const;

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

} // namespace epoll

#endif /* EPOLL_HPP_ */
