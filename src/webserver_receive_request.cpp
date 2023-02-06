#include "../includes/webserver.h"
#include <poll.h>

int check_if_done(Connection &connection) {
	if (connection._request._state == REQUEST_DONE) {
		return 1;
	}
	return 0;
}

void	receive_request(Connection &connection) {
	char								buffer[1024 * 8] = {0};
	//long								read_ret;
	std::string							value;
	struct pollfd 						poll_fd;

	std::cout << "[DEBUG]: receive_request" << std::endl;
	poll_fd.events = POLLIN | POLLOUT;
	poll_fd.revents = 0;
	poll_fd.fd = connection._socket;
	//read_ret = 1;

	poll(&poll_fd, 1, 0);
	if (poll_fd.revents & POLLHUP) {
		std::cout << "[DEBUG]: POLLHUP" << std::endl;
		//connection ended can close up
		//code
		return ;
	}
	if (poll_fd.revents & POLLIN) {
		connection._request._read_ret = read(connection._socket, buffer, 1024 * 8);
		if (connection._request._read_ret <= 0) {
			connection._request._state = REQUEST_CANCELLED;
			return;
		}
		connection._request._whole_request += std::string(buffer, buffer + connection._request._read_ret);
		std::cout << "[DEBUG]: whole request: " << connection._request._whole_request << std::endl;
	}

	if (connection._request._state == REQUEST_START)
		connection._request.set_method_url_version();
	if (connection._request._state == REQUEST_READING_HEADERS){
		if (connection._request._whole_request_at < connection._request._whole_request.size()) {
				connection._request.set_headers();
		}
		if (connection._request._content_length == 0) {
			connection._request._state = REQUEST_READING_DONE;
		}
	}
	if (connection._request._state == REQUEST_READING_BODY) {
		if (connection._request._whole_request_at < connection._request._whole_request.size()) {
			connection._request.set_body();
		}
	}
	if (connection._request._state == REQUEST_READING_DONE) {
		//close(connection._socket);
		connection._request._state = REQUEST_DONE;
	}
}