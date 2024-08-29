#include "epoll.hpp"
#include "utils.hpp"
#include <errno.h>
#include <stdint.h> // uint32_t
#include <unistd.h> // close

namespace epoll {

Epoll::Epoll() {
	epoll_fd_ = epoll_create1(EPOLL_CLOEXEC);
	if (epoll_fd_ == SYSTEM_ERROR) {
		throw std::runtime_error("epoll_create failed");
	}
}

Epoll::~Epoll() {
	if (epoll_fd_ != SYSTEM_ERROR) {
		close(epoll_fd_);
	}
}

namespace {

uint32_t ConvertToEpollEventType(event::Type type) {
	uint32_t ret_type = 0;

	if (type & event::EVENT_READ) {
		ret_type |= EPOLLIN;
	}
	if (type & event::EVENT_WRITE) {
		ret_type |= EPOLLOUT;
	}
	return ret_type;
}

uint32_t ConvertToEventType(uint32_t event) {
	uint32_t ret_type = event::EVENT_NONE;

	if (event & EPOLLIN) {
		ret_type |= event::EVENT_READ;
	}
	if (event & EPOLLOUT) {
		ret_type |= event::EVENT_WRITE;
	}
	// todo: tmp
	return ret_type;
}

event::Event ConvertToEventDto(const struct epoll_event &event) {
	event::Event ret_event;
	ret_event.fd   = event.data.fd;
	ret_event.type = ConvertToEventType(event.events);
	return ret_event;
}

} // namespace

// add socket_fd to epoll's interest list
void Epoll::Add(int socket_fd, event::Type type) {
	struct epoll_event ev = {};
	ev.events             = ConvertToEpollEventType(type);
	ev.data.fd            = socket_fd;
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, socket_fd, &ev) == SYSTEM_ERROR) {
		throw std::runtime_error("epoll_ctl failed");
	}
}

// remove socket_fd from epoll's interest list
void Epoll::Delete(int socket_fd) {
	// todo: error?
	epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, socket_fd, NULL);
}

int Epoll::CreateReadyList() {
	errno = 0;
	// todo: set timeout(ms)
	const int ready = epoll_wait(epoll_fd_, evlist_, MAX_EVENTS, 500);

	if (ready == SYSTEM_ERROR) {
		if (errno == EINTR) {
			return ready;
		}
		throw std::runtime_error("epoll_wait failed");
	}
	return ready;
}

// replace old_type with new_type
void Epoll::Replace(int socket_fd, const event::Type new_type) {
	struct epoll_event ev = {};
	ev.events             = ConvertToEpollEventType(new_type);
	ev.data.fd            = socket_fd;
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, socket_fd, &ev) == SYSTEM_ERROR) {
		throw std::runtime_error("epoll_ctl failed");
	}
}

// append old_type to new_type
void Epoll::Append(const event::Event &event, const event::Type new_type) {
	int socket_fd = event.fd;

	struct epoll_event ev = {};
	ev.events             = ConvertToEpollEventType(event.type) | ConvertToEpollEventType(new_type);
	ev.data.fd            = socket_fd;
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, socket_fd, &ev) == SYSTEM_ERROR) {
		throw std::runtime_error("epoll_ctl failed");
	}
}

event::Event Epoll::GetEvent(std::size_t index) const {
	if (index >= MAX_EVENTS) {
		throw std::out_of_range("evlist index out of range");
	}
	return ConvertToEventDto(evlist_[index]);
}

} // namespace epoll
