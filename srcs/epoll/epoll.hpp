#ifndef EPOLL_EPOLL_HPP_
#define EPOLL_EPOLL_HPP_

#include "event.hpp"
#include <cstddef>     // size_t
#include <sys/epoll.h> // epoll

namespace epoll {

class Epoll {
  public:
	typedef struct epoll_event EpollEvent;

	Epoll();
	~Epoll();

	void             Add(int socket_fd, event::Type type);
	void             Delete(int socket_fd);
	void             Replace(int socket_fd, event::Type new_type);
	void             Append(const event::Event &event, event::Type new_type);
	event::EventList GetEventList();

  private:
	// prohibit copy
	Epoll(const Epoll &other);
	Epoll &operator=(const Epoll &other);
	// function
	int CreateReadyList();
	// const
	static const int          SYSTEM_ERROR = -1;
	static const unsigned int MAX_EVENTS   = 10; // todo: remove
	// variables
	int          epoll_fd_;
	EpollEvent   evlist_[MAX_EVENTS]; // todo: remove
	unsigned int monitored_fd_count_;
};

} // namespace epoll

#endif /* EPOLL_EPOLL_HPP_ */
