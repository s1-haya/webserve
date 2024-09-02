#include "send.hpp"
#include "utils.hpp"
#include <cstdio>       // perror
#include <sys/socket.h> // send

namespace server {

Send::Send() {}

Send::~Send() {}

Send::SendResult Send::SendResponse(int client_fd, const Response &response) {
	SendResult send_result;

	ssize_t send_size = send(client_fd, response.c_str(), response.size(), 0);
	utils::Debug("Send", "send_size: ", send_size);
	if (send_size == SYSTEM_ERROR) {
		perror("send failed");
		send_result.Set(false);
		return send_result;
	}
	// Returns the substring starting from the send_size-th character.
	// If the entire response was sent, an empty string is returned.
	const Response unsent_response = response.substr(send_size);
	send_result.SetValue(unsent_response);
	return send_result;
}

} // namespace server
