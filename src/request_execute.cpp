#include "webserver.h"

int	check_method(Connection &connection) {
	for (int i = 0; i < connection._request._location_serv->accepted_methods.size(); i++) {
		if (connection._request._location_serv->accepted_methods[i] == connection._request.get_method())
			return 1;
	}
	return (0);
}

int set_location(Connection &connection) {
	string uri = connection._request.get_path();
	connection._request._location_serv = NULL;
	for (int i = 0; i < connection._server.locations.size(); i++) {
		if (connection._server.locations[i].location == uri) {
			connection._request._location_serv = &connection._server.locations[i];
			break;
		}
	}
}

int execute_request(Connection &connection) {

	set_location(connection);
	check_method(connection);


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