#include "webserver.h"

int	check_method(Connection &connection) {
	Request &request = connection._request;
	Response &response = connection._resp;
	for (size_t i = 0; i < response._location_serv->accepted_methods.size(); i++) {
		if (response._location_serv->accepted_methods[i] == request.get_method())
			return 0;
	}
	response._status_code = "405";
	//Maybe send errorpage here?
	return 1;
}

void set_location(Connection &connection) {
	Request &request = connection._request;
	Response &response = connection._resp;
	ConfigServer &server = connection._server;

	Location loc;
	string uri = request.get_path();
	response._location_serv = NULL;

	for (size_t i = 0; i < server.locations.size(); i++) {
		if (server.locations[i].location == uri) {
			response._location_serv = &server.locations[i];
			response._file_path = server.root + response._location_serv->index;
			return;
		}
	}

	connection._resp._status_code = "404";
	connection._resp.set_status_message("not found");

	//maybe send error page here?
	connection._resp._location_serv = NULL;
}

int execute_request(Connection &connection) {
	Request &request = connection._request;
	Response &response = connection._resp;
	ConfigServer &server = connection._server;
	//all checks set error code in response. If all checks are done errorcode needs to be checked before next fase.

	//preparing response
	set_location(connection);
	if (connection._resp._status_code == "404") {
		error_request(connection);
		return 1;//return error page
	}
	
	//check if method is allowed
	check_method(connection);
	if (connection._resp._status_code == "405")
		return 1;//return error page

	if (response._location_serv == NULL) {
		//return error page
	}
	response.set_client_socket(connection._socket);
	//check if I need to check the homepage here
	if (request.get_path() == "/") {
		response._file_path = response._location_serv->index;
	}
	response._file_path = server.root + response._location_serv->index;

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