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
#include "Cgi.hpp"
#include <dirent.h>     // DIR
#include <string>	   // string
#include <vector>	   // vector




int execute_request(Connection &connection) {

	//[INFO] Preparing response, error checking
	connection._response.set_client_socket(connection._socket);
	connection.set_location();
	cout << "DEBUG 1"<< endl;

	if (connection.check_response_status_error()) {
		error_request(connection);
			cout << "DEBUG 2"<< endl;
		return 1;
	}
	connection.check_method();
	if (connection.check_response_status_error()) {
		error_request(connection);
			cout << "DEBUG 3"<< endl;
		return 1;
	}
		cout << "DEBUG 4"<< endl;
	if (connection._request.get_method() == "GET") {
		cout << "DEBUG 5"<< endl;
		if (connection._response._location_server->autoindex == 1) {
			string index_location = connection._server.root + connection._response._location_server->location;
			autoindex_get(connection, index_location);
		}
		else if (connection._response._location_server->cgi == 1)
			cgi_get_request(connection);
		else {
			cout << "DEBUG 6"<< endl;
			get_request(connection);
		}
	}
	else if (connection._request.get_method() == "POST") {
		if (connection._response._location_server->cgi == 1)
			cgi_post_request(connection);
		else
			post_request(connection);
	}
	else if (connection._request.get_method() == "DELETE") {
		delete_request(connection);
	}
	return 0;
}
