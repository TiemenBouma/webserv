#include "../includes/webserver.h"
#include <poll.h>

void	receive_request(Connection &connection) {
	char								buffer[1024 * 8] = {0};
	long								read_ret;
	std::string							value;
	struct pollfd 						poll_fd;

	poll_fd.events = POLLIN | POLLOUT;
	poll_fd.revents = 0;
	poll_fd.fd = connection._socket;
	read_ret = 1;

	poll(&poll_fd, 1, 0);
	if (poll_fd.revents & POLLHUP) {
		//connection ended can close up
		//code
		return ;
	}
	if (connection._request.state == REQUEST_START) {
		if (poll_fd.revents & POLLIN) {
			read_ret = read(connection._socket, buffer, 1024 * 8);
			if (read_ret < 0) {
				connection._request.state = REQUEST_CANCELLED;
				return;
			}
			if (!read_ret) {
				connection._request.state = REQUEST_CANCELLED;
				return;
			}
			connection._request.whole_request += std::string(buffer, buffer + read_ret);
		}
		return;
	}
}