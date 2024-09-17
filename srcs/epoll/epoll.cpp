#include "epoll.hpp"
#include "system_exception.hpp"
#include "utils.hpp"
#include <cerrno>
#include <stdint.h> // uint32_t
#include <unistd.h> // close

namespace epoll {

Epoll::Epoll() : monitored_fd_count_(0) {
	epoll_fd_ = epoll_create1(EPOLL_CLOEXEC);
	if (epoll_fd_ == SYSTEM_ERROR) {
		throw utils::SystemException(errno);
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
int Epoll::CreateEventReadyList() {
	errno = 0;
	// todo: set timeout(ms)
	const int ready = epoll_wait(epoll_fd_, evlist_, MAX_EVENTS, 500);
	if (ready == SYSTEM_ERROR) {
		if (errno == EINTR) {
			return 0;
		}
		throw utils::SystemException(errno);
	}
	return ready;
}

event::EventList Epoll::GetEventList() {
	const int ready_list_size = CreateEventReadyList();

	event::EventList events;
	for (std::size_t i = 0; i < static_cast<std::size_t>(ready_list_size); ++i) {
		events.push_back(ConvertToEvent(evlist_[i]));
	}
	return events;
}

// add new socket_fd to epoll's interest list
void Epoll::Add(int socket_fd, event::Type type) {
	EpollEvent ev = {};
	ev.events     = ConvertToEpollEventType(type);
	ev.data.fd    = socket_fd;
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, socket_fd, &ev) == SYSTEM_ERROR) {
		throw utils::SystemException(errno);
	}
	++monitored_fd_count_;
}

// remove socket_fd from epoll's interest list
void Epoll::Delete(int socket_fd) {
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, socket_fd, NULL) == SYSTEM_ERROR) {
		throw utils::SystemException(errno);
	}
	--monitored_fd_count_;
}

// replace old_type with new_type
void Epoll::Replace(int socket_fd, const event::Type new_type) {
	EpollEvent ev = {};
	ev.events     = ConvertToEpollEventType(new_type);
	ev.data.fd    = socket_fd;
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, socket_fd, &ev) == SYSTEM_ERROR) {
		throw utils::SystemException(errno);
	}
}

// append new_type to old_type
void Epoll::Append(const event::Event &event, const event::Type new_type) {
	int socket_fd = event.fd;

	EpollEvent ev = {};
	ev.events     = ConvertToEpollEventType(event.type) | ConvertToEpollEventType(new_type);
	ev.data.fd    = socket_fd;
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, socket_fd, &ev) == SYSTEM_ERROR) {
		throw utils::SystemException(errno);
	}
}

} // namespace epoll
