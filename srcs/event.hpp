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
