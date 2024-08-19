#include "message.hpp"

namespace server {

Message::Message() {}

Message::~Message() {}

Message::Message(const Message &other) {
	*this = other;
}

Message &Message::operator=(const Message &other) {
	if (this != &other) {
	}
	return *this;
}

} // namespace server
