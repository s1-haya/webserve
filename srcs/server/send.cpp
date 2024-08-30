#include "send.hpp"
#include <cstdio>       // perror
#include <sys/socket.h> // send

#include <iostream>

namespace server {

Send::Send() {}

Send::~Send() {}

Send::SendResult Send::SendResponse(int client_fd, const Response &response) {
	SendResult send_result;

	ssize_t send_ret = send(client_fd, response.c_str(), response.size(), 0);
	if (send_ret == SYSTEM_ERROR) {
		perror("send failed");
		send_result.Set(false);
		return send_result;
	}
	const Response unsent_response = response.substr(send_ret);
	send_result.SetValue(unsent_response);
	return send_result;
}

} // namespace server
