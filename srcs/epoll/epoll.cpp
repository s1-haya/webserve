#include "epoll.hpp"
#include "system_exception.hpp"
#include "utils.hpp"
#include <cstring> // strerror
#include <errno.h>
#include <stdint.h> // uint32_t
#include <unistd.h> // close

namespace epoll {

Epoll::Epoll() {
	epoll_fd_ = epoll_create1(EPOLL_CLOEXEC);
	if (epoll_fd_ == SYSTEM_ERROR) {
		throw SystemException("epoll_create failed: " + std::string(strerror(errno)));
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

event::Event ConvertToEventDto(const struct epoll_event &event) {
	event::Event ret_event;
	ret_event.fd   = event.data.fd;
	ret_event.type = ConvertToEventType(event.events);
	return ret_event;
}

} // namespace

// add new socket_fd to epoll's interest list
void Epoll::Add(int socket_fd, event::Type type) {
	struct epoll_event ev = {};
	ev.events             = ConvertToEpollEventType(type);
	ev.data.fd            = socket_fd;
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, socket_fd, &ev) == SYSTEM_ERROR) {
		throw SystemException("epoll_ctl add failed: " + std::string(strerror(errno)));
	}
}

// remove socket_fd from epoll's interest list
void Epoll::Delete(int socket_fd) {
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, socket_fd, NULL) == SYSTEM_ERROR) {
		throw SystemException("epoll_ctl delete failed: " + std::string(strerror(errno)));
	}
}

// epoll_wait() always monitors EPOLLHUP and EPOLLERR, so no need to explicitly set them in events.
int Epoll::CreateReadyList() {
	errno = 0;
	// todo: set timeout(ms)
	const int ready = epoll_wait(epoll_fd_, evlist_, MAX_EVENTS, 500);
	if (ready == SYSTEM_ERROR) {
		if (errno == EINTR) {
			return ready;
		}
		throw SystemException("epoll_wait failed: " + std::string(strerror(errno)));
	}
	return ready;
}

// replace old_type with new_type
void Epoll::Replace(int socket_fd, const event::Type new_type) {
	struct epoll_event ev = {};
	ev.events             = ConvertToEpollEventType(new_type);
	ev.data.fd            = socket_fd;
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, socket_fd, &ev) == SYSTEM_ERROR) {
		throw SystemException("epoll_ctl replace failed: " + std::string(strerror(errno)));
	}
}

// append new_type to old_type
void Epoll::Append(const event::Event &event, const event::Type new_type) {
	int socket_fd = event.fd;

	struct epoll_event ev = {};
	ev.events             = ConvertToEpollEventType(event.type) | ConvertToEpollEventType(new_type);
	ev.data.fd            = socket_fd;
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, socket_fd, &ev) == SYSTEM_ERROR) {
		throw SystemException("epoll_ctl append failed: " + std::string(strerror(errno)));
	}
}

event::Event Epoll::GetEvent(std::size_t index) const {
	if (index >= MAX_EVENTS) {
		throw std::logic_error("evlist index out of range");
	}
	return ConvertToEventDto(evlist_[index]);
}

} // namespace epoll
