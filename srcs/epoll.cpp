#include "epoll.hpp"
#include <errno.h>
#include <stdint.h> // uint32_t
#include <unistd.h> // close

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
	uint32_t ConvertToEpollEventType(EventType type) {
		if (type == EVENT_READ) {
			return EPOLLIN;
		}
		// todo: tmp
		return 0;
	}
} // namespace

void Epoll::AddNewConnection(int socket_fd, EventType type) {
	struct epoll_event ev;
	ev.events  = ConvertToEpollEventType(type);
	ev.data.fd = socket_fd;
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, socket_fd, &ev) == SYSTEM_ERROR) {
		throw std::runtime_error("epoll_ctl failed");
	}
}

// todo: error?
void Epoll::DeleteConnection(int socket_fd) {
	epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, socket_fd, NULL);
}

int Epoll::CreateReadyList() {
	errno           = 0;
	const int ready = epoll_wait(epoll_fd_, evlist_, MAX_EVENTS, -1);
	if (ready == SYSTEM_ERROR) {
		if (errno == EINTR) {
			return ready;
		}
		throw std::runtime_error("epoll_wait failed");
	}
	return ready;
}

namespace {
	EventType ConvertToEventType(uint32_t event) {
		if (event & EPOLLIN) {
			return EVENT_READ;
		}
		// todo: tmp
		return EVENT_NONE;
	}

	Event ConvertToEventDto(const struct epoll_event &event) {
		Event ret_event;
		ret_event.fd   = event.data.fd;
		ret_event.type = ConvertToEventType(event.events);
		return ret_event;
	}
} // namespace

const Event Epoll::GetEvent(std::size_t index) const {
	if (index >= MAX_EVENTS) {
		throw std::out_of_range("evlist index out of range");
	}
	return ConvertToEventDto(evlist_[index]);
}
