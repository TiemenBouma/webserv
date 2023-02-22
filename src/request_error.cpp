#include "webserver.h"
#include "typedef.h"
#include <fstream>
#include <sstream>


#include <unistd.h>

string find_error_page(Connection &connection) {
	stringstream ss(connection._resp._status_code);
	int error_code;
	ss >> error_code;
	string error_page = connection._server.root + connection._server.error_pages[error_code];

	return error_page;
}

void error_request(Connection &connection) {
    std::ifstream file;
    std::string error_page;

	//[INFO] find the error page
	error_page = find_error_page(connection);
   
    //[INFO] Open the file in binary mode
    file.open(error_page.c_str(), std::ios::binary);

    if (file.is_open()) {

        //[INFO] Determine the MIME type of the file
        connection._resp.set_header_content_type(error_page);

        //[INFO] Get the file size
		connection._resp.set_header_content_length(file);

		//[INFO] WRITE/SEND THE HEADERS
		std::cout << "SERVER: Sending ERROR response: \n" << std::endl;
		std::string response_str = connection._resp.serialize_headers();
		connection._resp.write_to_socket(response_str.c_str(), response_str.size());

		//[INFO] write/send the body of the response in chunks for speed
        const std::streamsize BUFSIZE = 8192;
        char buffer[BUFSIZE];
        std::streamsize n;
        while ((n = file.read(buffer, BUFSIZE).gcount()) > 0) {
			write(1, buffer, n);
            connection._resp.write_to_socket(buffer, n);

        }
		cout << "SERVER: End error response" << endl;
        file.close();
    } 
	else
        std::cout << "SERVER: Error open error_page" << std::endl;
}