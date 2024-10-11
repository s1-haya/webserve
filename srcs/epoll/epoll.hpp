#ifndef EPOLL_EPOLL_HPP_
#define EPOLL_EPOLL_HPP_

#include "event.hpp"
#include <cstddef>     // size_t
#include <sys/epoll.h> // epoll
#include <vector>

namespace epoll {

class Epoll {
  public:
	typedef struct epoll_event      EpollEvent;
	typedef std::vector<EpollEvent> EpollEventVector;

	Epoll();
	~Epoll();

	event::EventList GetEventList();
	void             Add(int socket_fd, event::Type type);
	void             Delete(int socket_fd);
	void             Replace(int socket_fd, uint32_t new_type);
	void             Append(const event::Event &event, event::Type new_type);

  private:
	// prohibit copy
	Epoll(const Epoll &other);
	Epoll &operator=(const Epoll &other);
	// function
	EpollEventVector CreateEventReadyList();
	// const
	static const int SYSTEM_ERROR = -1;
	static const int WAIT_TIMEOUT = 500; // ms
	// variables
	int          epoll_fd_;
	unsigned int monitored_fd_count_;
};

} // namespace epoll

#endif /* EPOLL_EPOLL_HPP_ */
