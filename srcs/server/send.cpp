#include "send.hpp"
#include "utils.hpp"
#include <cerrno>
#include <cstring>     // strerror
#include <sys/types.h> // ssize_t
#include <unistd.h>    // write

namespace server {

Send::Send() {}

Send::~Send() {}

Send::SendResult Send::SendStr(int client_fd, const std::string &send_str) {
	SendResult send_result;

	ssize_t send_size = write(client_fd, send_str.c_str(), send_str.size());
	utils::Debug("Send", "send_size: ", send_size);
	if (send_size == SYSTEM_ERROR) {
		utils::PrintError(__func__, strerror(errno));
		send_result.Set(false);
		return send_result;
	}
	// Returns the substring starting from the send_size-th character.
	// If the entire send_str was sent, an empty string is returned.
	const std::string unsent_str = send_str.substr(send_size);
	send_result.SetValue(unsent_str);
	return send_result;
}

} // namespace server
