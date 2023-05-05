#include "webserver.h"
#include "typedef.h"
#include "Cgi.hpp"
#include <fstream>
#include <unistd.h>

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

int get_request(Connection &connection) {
    std::ifstream	file;
    std::string		file_dir;
	Location		currect_loc;

    //[INFO] Open the file in binary mode

    file.open(connection._response._file_path.c_str(), std::ios::binary);

    if (file.is_open()) {

        //[INFO] Determine the MIME type of the file
        connection._response.set_header_content_type(connection._response._file_path);

        //[INFO] Get the file size
		connection._response.set_header_content_length_file(file);

		//[INFO] WRITE/SEND THE HEADERS
		//std::cout << "SERVER: Sending GET response: \n" << std::endl;
		std::string response_string = connection._response.serialize_headers();
		//std::cout << "DEBUG send response:\n" << response_string << std::endl;
		ssize_t ret = connection._response.write_to_socket(response_string.c_str(), response_string.size());
		if (ret == -1) {
			return 1;
		}

		//[INFO] write/send the body of the response in chunks for speed
        const std::streamsize BUFSIZE = BUFFER_SIZE_8K;
        char buffer[BUFSIZE];
        std::streamsize n;
        while ((n = file.read(buffer, BUFFER_SIZE_8K).gcount()) > 0) {
            ret = connection._response.write_to_socket(buffer, n);
			if (ret == -1) {
				file.close();
				return 1;
		}
        }

		//[INFO] END OF GET REQUEST
        file.close();
    } 
	else {//Server side error
        connection._response.set_status_code("500");
		cerr << "[SERVER] error. Cant open file: " << connection._response._file_path << " check config" << endl;
		error_request(connection);
    }
    return 0;
}
