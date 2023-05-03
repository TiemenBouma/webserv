/**
 * @file webserver_connections_receive.cpp
 * @author tbouma
 * @brief webserv 42 network
 * @version 0.1
 * @date 2023-03-29
 */

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

	poll_fd.events = POLLIN;
	poll_fd.revents = 0;
	poll_fd.fd = connection._socket;

	//checking on timeout, returning if time is over 30 sec
	if (connection.check_time_out())
		return;

	//TIMEOUT 1ms for poll might not be good or allowed.
	if (poll(&poll_fd, 1, 1) == -1)
		return;
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
		cout << "[DEBUG]whole request: " << connection._request._whole_request << endl;
	}
	else
		connection._request._state = REQUEST_CANCELLED;
	if (connection._request._state == REQUEST_START) {
		connection.set_method_url_version();
	}

	if (connection._request._state == REQUEST_READING_HEADERS) {
		connection.reading_headers();
	}

	if (connection._request._state == REQUEST_SETTING_HEADERS) {
		connection.set_headers();
	}

	if (connection._request._state == REQUEST_READING_BODY) {
		if (connection._request._whole_request_at < connection._request._whole_request.size()) {
			connection.set_body();
		}
	}
}
