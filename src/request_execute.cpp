#include "webserver.h"

int execute_request(Connection &connection) {
	connection._resp.set_client_socket(connection._socket);
	if (connection._request.get_method() == "GET") {
		std::cout << "DEBUG: GET request" << std::endl;
		get_request(connection);
	}
	else if (connection._request.get_method() == "POST") {
		std::cout << "DEBUG: POST request" << std::endl;
		post_request(connection);
	}
	// else if (client_req.get_method() == "DELETE")

	// else
	// 	error;
	return 0;
}