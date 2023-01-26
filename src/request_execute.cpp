#include "../includes/webserver.h"

int execute_request(Request &client_req, Response &server_resp) {
	if (client_req.get_method() == "GET") {
		std::cout << "DEBUG: GET request" << std::endl;
		get_request(client_req, server_resp);
	}
	else if (client_req.get_method() == "POST") {
		std::cout << "DEBUG: POST request" << std::endl;
		post_request(client_req, server_resp);
	}
	// else if (client_req.get_method() == "DELETE")

	// else
	// 	error;
	return 0;
}