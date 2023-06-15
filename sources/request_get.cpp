#include "webserver.h"
#include "typedef.h"
#include "Cgi.hpp"
#include <fstream>
#include <unistd.h>

#include <sys/socket.h>

/* 
questions:
Is content length neccesary?
What error message should be given on write_to_socket failure?
Add environment variables or not?
*/

void	cgi_get_request(Connection& connection)
{
	int					pid;
	Cgi					cgi;
	const std::string	headers = "HTTP/1.1 200 OK\n";

	if ((pid = fork()) == -1)
	{
		connection._response.set_status_code("500");
		error_request(connection);
	}
	else if (pid == 0)
	{
		try {
			if (access(connection._response._file_path.c_str(), X_OK) == -1)
				throw(Cgi::CgiSystemFailure());
			//[INFO] run cgi
			std::string cgi_out = cgi.cgi(connection._response._file_path, Cgi::make_env(connection._server, *(connection._response._location_server), "GET"), "");
			connection._response.set_header_content_length_string(cgi_out);
			ssize_t ret = connection._response.write_to_socket(headers.c_str(), headers.size());
			if (ret == -1) {
				std::cout << "[ERROR] in cgi headers" << std::endl;
				exit(1);
			}
			ret = connection._response.write_to_socket(cgi_out.c_str(), cgi_out.size());
			if (ret == -1) {
				std::cout << "[ERROR] in cgi body" << std::endl;
				exit(1);
			}
		}
		catch (std::exception& e) {
			std::cout << "Cgi exception caught." << std::endl;
			std::cout << e.what() << std::endl;
			connection._response.set_status_code("500");
			error_request(connection);
		}
		exit(1);
	}

	// std::cout << "cgi output: " << cgi_out << std::endl;
}

// int gci_get_request() {

// }


// ssize_t demo_send_all(int socket, const void *buffer, size_t length, int flags) {
// 	ssize_t total_sent = 0;
// 	while (total_sent < length) {
// 		ssize_t sent = send(socket, (char*)buffer + total_sent, length - total_sent, flags);
// 		if (sent == -1) {
// 			// send would block, return how much was sent
// 			return total_sent;
// 		}
// 		total_sent += sent;
// 	}
// 	return total_sent; // total_sent should be equal to length
// }

int get_request(Connection &connection) {
	ssize_t ret;

    //[INFO] Open the file in binary mode
	if (!connection._response._ifstream_response.is_open()) {
		cout << "DEBUG: Opening file" << endl;

    	connection._response._ifstream_response.open(connection._response._file_path.c_str(), std::ios::binary);
       
		if (!connection._response._ifstream_response.is_open()) {
			connection._response.set_status_code("500");
			cerr << "[SERVER] error. Cant open file: " << connection._response._file_path << " check config" << endl;
			error_request(connection);
			return 1;
    	}
	   	cout << "DEBUG1" << endl;
	    //[INFO] Determine the MIME type of the file
        connection._response.set_header_content_type(connection._response._file_path);

	   	cout << "DEBUG2" << endl;
        //[INFO] Get the file size
		connection._response.set_header_content_length_file(connection._response._ifstream_response);
	
		//[INFO] WRITE/SEND THE HEADERS
		//std::cout << "SERVER: Sending GET response: \n" << std::endl;
		std::string response_string = connection._response.serialize_headers();
		//std::cout << "DEBUG send response:\n" << response_string << std::endl;
		ret = connection._response.write_to_socket(response_string.c_str(), response_string.size());
		if (ret == -1) {
			return 1;
		}
	}

    if (connection._response._ifstream_response.is_open()) {


		connection._response.set_content_from_file();
		//connection._response._ifstream_response.seekg(0, std::ios::beg);


		//[INFO] write/send the body of the response in chunks

		ret = connection._response.body_send_all(connection._socket, connection._response._body.c_str(), connection._response._body.size(), 0);

		cout << "DEBUG: body send: " << ret << endl;
		cout << "DEBUG size body: " << connection._response._header_content_length << endl;

    } 
	else {//Server side error
        connection._response.set_status_code("500");
		cerr << "[SERVER] error. Cant open file: " << connection._response._file_path << " check config" << endl;
		error_request(connection);
    }
    return 0;
}
