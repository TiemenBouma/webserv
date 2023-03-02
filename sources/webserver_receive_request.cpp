#include "webserver.h"
#include <poll.h>
#include <unistd.h>

int check_if_done(Connection &connection) {
	if (connection._request._state == REQUEST_DONE) {
		return 1;
	}
	return 0;
}

void	receive_request(Connection &connection) {
	char								buffer[1024 * 8] = {0};
	std::string							value;
	struct pollfd 						poll_fd;

	poll_fd.events = POLLIN | POLLHUP;
	poll_fd.revents = 0;
	poll_fd.fd = connection._socket;

	//TIMEOUT 1ms for poll might not be good or allowed.
	poll(&poll_fd, 1, 1);

	//-----------------DEBUGING POLL
	// if (poll_fd.revents & POLLIN) {
	// 	std::cout << "[DEBUG]: POLLIN in receive is working" << std::endl;
	// } else {
	// 	std::cout << "[DEBUG]: POLLIN in receive is not working" << std::endl;
	// }
	//----------------END DEBUGING POLL

	if (poll_fd.revents & POLLHUP) {
		cout << "[DEBUG]POLLHUP" << endl;
		if (connection._request._state == REQUEST_READING_DONE)
			connection._request._state = REQUEST_DONE;
		else
			connection._request._state = REQUEST_CANCELLED;
		return;
	}

	if (poll_fd.revents & POLLIN) {
		int read_ret = read(connection._socket, buffer, 1024 * 8);
		if (read_ret < 0) {
			cout << "[SERVER]read() ERROR" << endl;
			connection._request._state = REQUEST_CANCELLED;
			return;
		}
		if (read_ret == 0) {
			if (connection._request._state == REQUEST_READING_DONE) {
				connection._request._state = REQUEST_DONE;
			}
			else {
				cout << "[SERVER]read() CANCELLED" << endl;
				connection._request._state = REQUEST_CANCELLED;
			}
			return;
		}

		connection._request._whole_request += std::string(buffer, buffer + read_ret);
		//cout << "[DEBUG]whole request: " << connection._request._whole_request << endl;
	}
	else {
		connection._request._state = REQUEST_CANCELLED;
		return;
	}

	if (connection._request._state == REQUEST_START)
		connection._request.set_method_url_version();
	if (connection._request._state == REQUEST_READING_HEADERS) {
		connection._request.reading_headers();
	}
	if (connection._request._state == REQUEST_SETTING_HEADERS) {
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
		connection._request._state = REQUEST_DONE;
	}
}