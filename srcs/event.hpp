#ifndef EVENT_HPP_
#define EVENT_HPP_

enum EventType {
	EVENT_NONE, // todo: tmp
	EVENT_READ,
	EVENT_HANGUP,
	EVENT_ERR
};

struct Event {
	int       fd;
	EventType type;
};

#endif /* EVENT_HPP_ */
