#include "webserver.h"

int	check_method(Connection &connection) {
	//debug
	for (size_t i = 0; i < connection._server.locations.size(); i++) {
		cout << "DEBUG: check_method loc: " << connection._server.locations[i].location << endl;
	}
	for (size_t i = 0; i < connection._request._location_serv->accepted_methods.size(); i++) {
		cout << "DEBUG: check_method method: " << connection._request._location_serv->accepted_methods[i] << endl;
	}
	for (size_t i = 0; i < connection._request._location_serv->accepted_methods.size(); i++) {
		if (connection._request._location_serv->accepted_methods[i] == connection._request.get_method())
			return 0;
	}
	connection._resp._status_code = "405";
	return 1;
}

void set_location(Connection &connection) {
	Location loc;
	string uri = connection._request.get_path();
	connection._request._location_serv = NULL;
	for (size_t i = 0; i < connection._server.locations.size(); i++) {
		if (connection._server.locations[i].location == uri) {
			connection._resp._location_serv = &connection._server.locations[i];
			return;
		}
	}
	connection._resp._status_code = "404";
	connection._resp._location_serv = NULL;
}

int execute_request(Connection &connection) {
	cout << "DEBUG1: execute_request" << endl;
	Request &req_ref = connection._request;
	Response &resp_ref = connection._resp;
	ConfigServer &server_ref = connection._server;
	//all checks set error code in response. If all checks are done errorcode needs to be checked before next fase.
	cout << "DEBUG2: execute_request" << endl;

	//check if method is allowed
	if (check_method(connection)) 
	cout << "DEBUG3: execute_request" << endl;
	
	//preparing response
	set_location(connection);
	cout << "DEBU4: execute_request" << endl;

	if (resp_ref._location_serv == NULL) {
		//return error page
	}
	resp_ref.set_client_socket(connection._socket);
	if (req_ref.get_path() == "/") {
		req_ref.set_path(resp_ref._location_serv->index);
	}
	resp_ref._file_path = server_ref.root + req_ref.get_path();



	if (connection._request.get_method() == "GET") {
		std::cout << "DEBUG: GET request" << std::endl;
		get_request(connection);
	}
	else if (connection._request.get_method() == "POST") {
		std::cout << "DEBUG: POST request" << std::endl;
		post_request(connection);
	}
	// else if (client_req.get_method() == "DELETE")

	return 0;
}