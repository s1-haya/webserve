#include "epoll.hpp"
#include "system_exception.hpp"
#include "utils.hpp"
#include <cerrno>
#include <cstring>  // strerror
#include <stdint.h> // uint32_t
#include <unistd.h> // close

namespace epoll {

Epoll::Epoll() : monitored_fd_count_(0) {
	epoll_fd_ = epoll_create1(EPOLL_CLOEXEC);
	if (epoll_fd_ == SYSTEM_ERROR) {
		throw SystemException("epoll_create1 failed: " + std::string(std::strerror(errno)));
	}
}

Epoll::~Epoll() {
	if (epoll_fd_ != SYSTEM_ERROR) {
		close(epoll_fd_);
	}
}

namespace {

uint32_t ConvertToEpollEventType(uint32_t type) {
	uint32_t ret_type = 0;

	if (type & event::EVENT_READ) {
		ret_type |= EPOLLIN;
	}
	if (type & event::EVENT_WRITE) {
		ret_type |= EPOLLOUT;
	}
	return ret_type;
}

uint32_t ConvertToEventType(uint32_t type) {
	uint32_t ret_type = event::EVENT_NONE;

	if (type & EPOLLIN) {
		ret_type |= event::EVENT_READ;
	}
	if (type & EPOLLOUT) {
		ret_type |= event::EVENT_WRITE;
	}
	if (type & EPOLLERR) {
		ret_type |= event::EVENT_ERROR;
	}
	if (type & EPOLLHUP) {
		ret_type |= event::EVENT_HANGUP;
	}
	return ret_type;
}

event::Event ConvertToEvent(const epoll::Epoll::EpollEvent &event) {
	event::Event ret_event;
	ret_event.fd   = event.data.fd;
	ret_event.type = ConvertToEventType(event.events);
	return ret_event;
}

} // namespace

// epoll_wait() always monitors EPOLLHUP and EPOLLERR, so no need to explicitly set them in events.
Epoll::EpollEventVector Epoll::CreateEventReadyList() {
	EpollEventVector events(monitored_fd_count_);

	errno = 0;
	// todo: set timeout(ms)
	const int ready_list_size = epoll_wait(epoll_fd_, events.data(), monitored_fd_count_, 500);
	if (ready_list_size == SYSTEM_ERROR) {
		if (errno == EINTR) {
			events.clear();
			return events;
		}
		throw SystemException("epoll_wait failed: " + std::string(std::strerror(errno)));
	}
	events.resize(ready_list_size);
	return events;
}

event::EventList Epoll::GetEventList() {
	EpollEventVector ready_events = CreateEventReadyList();
	event::EventList event_list;

	typedef EpollEventVector::const_iterator Itr;
	for (Itr it = ready_events.begin(); it != ready_events.end(); ++it) {
		event_list.push_back(ConvertToEvent(*it));
	}
	return event_list;
}

// add new socket_fd to epoll's interest list
void Epoll::Add(int socket_fd, event::Type type) {
	EpollEvent ev = {};
	ev.events     = ConvertToEpollEventType(type);
	ev.data.fd    = socket_fd;
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, socket_fd, &ev) == SYSTEM_ERROR) {
		throw SystemException("epoll_ctl add failed: " + std::string(std::strerror(errno)));
	}
	++monitored_fd_count_;
}

// remove socket_fd from epoll's interest list
void Epoll::Delete(int socket_fd) {
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, socket_fd, NULL) == SYSTEM_ERROR) {
		throw SystemException("epoll_ctl delete failed: " + std::string(std::strerror(errno)));
	}
	--monitored_fd_count_;
}

// replace old_type with new_type
// allows multiple event types, e.g., event::EVENT_READ | event::EVENT_WRITE.
void Epoll::Replace(int socket_fd, uint32_t new_type) {
	EpollEvent ev = {};
	ev.events     = ConvertToEpollEventType(new_type);
	ev.data.fd    = socket_fd;
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, socket_fd, &ev) == SYSTEM_ERROR) {
		throw SystemException("epoll_ctl replace failed: " + std::string(std::strerror(errno)));
	}
}

// append new_type to old_type
void Epoll::Append(const event::Event &event, const event::Type new_type) {
	int socket_fd = event.fd;

	EpollEvent ev = {};
	ev.events     = ConvertToEpollEventType(event.type) | ConvertToEpollEventType(new_type);
	ev.data.fd    = socket_fd;
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, socket_fd, &ev) == SYSTEM_ERROR) {
		throw SystemException("epoll_ctl append failed: " + std::string(std::strerror(errno)));
	}
}

} // namespace epoll
