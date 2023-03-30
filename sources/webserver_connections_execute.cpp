/**
 * @file webserver_connections_execute.cpp
 * @author tbauma
 * 
 * @brief webserver 42 network
 * @version 0.2
 * 
 * @date 2023-03-29
 */

#include "webserver.h"

int execute_request(Connection &connection) {

	//[INFO] Preparing response, error checking
	connection._response.set_client_socket(connection._socket);
	connection.set_location();
	if (connection._response._status_code != "200") {
		error_request(connection);
		return 1;
	}
	connection.check_method();
	if (connection._response._status_code != "200") {
		error_request(connection);
		return 1;
	}

	if (connection._request.get_method() == "GET") {
		// if (connection._response._location_serv->cgi == 1)
		// 	gci_get_request();
		// else
			get_request(connection);
	}
	else if (connection._request.get_method() == "POST") {
		post_request(connection);
	}
	else if (connection._request.get_method() == "DELETE") {
		delete_request(connection);
	}
	return 0;
}
