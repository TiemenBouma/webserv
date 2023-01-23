#include "Request.hpp"
#include "../includes/webserver.h"
#include "Response.hpp"
#include <fstream>
#include <string>
#include <iostream>

int get_request(Request &req, Response &resp) {
    std::ifstream file;
    std::string file_dir;

	//DETERMINE THE FILE TO OPEN
    if (req.get_url() == "/") {
        file_dir = HOMEPAGE_FILE;
    } else {
        file_dir = "../data" + req.get_url();
    }

    // Open the file in binary mode
    file.open(file_dir.c_str(), std::ios::binary);


    if (file.is_open()) {
        // Determine the MIME type of the file
	std::cout << "DEBUG: open file" << std::endl;
        resp.set_header_content_type(file_dir);
	std::cout << "DEBUG: after open file" << std::endl;

        // Get the file size
		resp.set_header_content_length(file);


		//WRITE/SEND THE HEADERS
		std::cout << "SERVER: Sending GET response: \n" << std::endl;
		std::string response = resp.serialize_headers();
		std::cout << "DEBUG send response:" << response << std::endl;
		resp.write_to_socket(response.c_str(), response.size());

		//write/send the body of the response in chunks for speed
        const std::streamsize BUFSIZE = 8192;
        char buffer[BUFSIZE];
        std::streamsize n;
        while ((n = file.read(buffer, BUFSIZE).gcount()) > 0) {
			std::cout << "DEBUG: writing body: " << buffer <<  std::endl;
            resp.write_to_socket(buffer, n);
        }

		//END OF GET REQUEST
        file.close();
    } else {
        std::cout << "Error opening file 404 error" << std::endl;
        return 404;
    }
    return 200;
}