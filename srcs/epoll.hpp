#ifndef EPOLL_HPP_
#define EPOLL_HPP_

#include "debug.hpp"   // todo: tmp
#include <sys/epoll.h> // epoll

// singlton??
class Epoll {
  public:
	Epoll();
	~Epoll();
	void AddNewConnection(int socket_fd);
	void DeleteConnection(int socket_fd);
	int  CreateReadyList();
	// getter
	const struct epoll_event &GetEvent(std::size_t index) const;

  private:
	// prohibit copy
	Epoll(const Epoll &other);
	Epoll &operator=(const Epoll &other);
	// const
	static const int          kSystemErr = -1;
	static const unsigned int kMaxEvents = 10;
	// variables
	int                epoll_fd_;
	struct epoll_event ev_;
	struct epoll_event evlist_[kMaxEvents];
};

#endif /* EPOLL_HPP_ */
