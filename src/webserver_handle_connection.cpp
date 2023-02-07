#include "webserver.h"
// #include "../includes/webserver.h"

void handle_connection(Connection &connection) {
	(void)connection;

	//std::cout << "DEBUG: Handling connection" << std::endl;
						// [INFO] READ REQUEST
						//std::stringstream request_data;
						//std::cout << "DEBUG: Reading request" << std::endl;
						//read_request(client_socket, request_data);
						//std::cout << "DEBUG: Reading request finished" << std::endl;


	//[INFO]PARSE REQUEST in Request class constructor
	// std::cout << "DEBUG: Parsing request" << std::endl;
	// Request client_request(request_data, mime_types, mime_types_rev);
	// std::cout << "DEBUG: Parsing request finiched" << std::endl;
	
	// //[INFO]Construct Response

	// Response server_resp(mime_types, mime_types_rev);
	//server_resp.set_client_socket(client_socket);

	// [INFO] Here we need a function to decide what response we do
	//now an example just with GET
	//std::cout << "DEBUG: Executing request" << std::endl;
	//execute_request(client_request, server_resp);
	//close(connection._socket);
}