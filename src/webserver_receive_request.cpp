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

	std::cout << "[DEBUG]: receive_request" << std::endl;
	poll_fd.events = POLLIN | POLLHUP;
	poll_fd.revents = 0;
	poll_fd.fd = connection._socket;

	poll(&poll_fd, 1, 1);

	//-----------------DEBUGING POLL
	if (poll_fd.revents & POLLIN) {
		std::cout << "[DEBUG]: POLLIN in receive is working" << std::endl;
	} else {
		std::cout << "[DEBUG]: POLLIN in receive is not working" << std::endl;
	}
	//----------------END DEBUGING POLL

	if (poll_fd.revents & POLLHUP) {
		std::cout << "[DEBUG]: POLLHUP active REQUEST CANCELLED" << std::endl;
		connection._request._error_log += "POLLHUB Error\n";
		connection._request._state = REQUEST_CANCELLED; //just for now cancel request, later need to check if correct.
		//connection ended can close up
		//code
		return ;
	}
	if (poll_fd.revents & POLLIN) {
		//cout << "[DEBUG]: start read" << endl;
		connection._request._read_ret = read(connection._socket, buffer, 1024 * 8);
		//cout << "[DEBUG] end read" << endl;
		if (connection._request._read_ret <= 0) {
			connection._request._state = REQUEST_CANCELLED;
			connection._request._error_log += "Read Error\n";
			return;
		}
		connection._request._whole_request += std::string(buffer, buffer + connection._request._read_ret);
		std::cout << "[DEBUG]: whole request:\n" << connection._request._whole_request << std::endl;
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
		connection._request._state = REQUEST_DONE;
	}
}