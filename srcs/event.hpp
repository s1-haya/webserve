#ifndef EVENT_HPP_
#define EVENT_HPP_

#include <stdint.h> // uint32_t

enum EventType {
	EVENT_NONE   = 1 << 0, // todo: tmp
	EVENT_READ   = 1 << 1,
	EVENT_WRITE  = 1 << 2,
	EVENT_ERROR  = 1 << 3,
	EVENT_HANGUP = 1 << 4
};

struct Event {
	int      fd;
	uint32_t type;
};

#endif /* EVENT_HPP_ */
