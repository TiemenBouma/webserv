#include "Request.hpp"
#include "../includes/webserver.h"
#include "Response.hpp"
#include <fstream>
#include <string>
#include <iostream>

int get_request(Request &client_req, Response &server_resp, ConfigFile configData)  {
    std::ifstream file;
    std::string file_dir;

	//[INFO] DETERMINE THE FILE TO OPEN
    if (client_req.get_path() == "/") {
        file_dir = HOMEPAGE_FILE;
    } else {
        file_dir = configData.root + client_req.get_path();
    }

    //[INFO] Open the file in binary mode
    file.open(file_dir.c_str(), std::ios::binary);


    if (file.is_open()) {
        //[INFO] Determine the MIME type of the file
        server_resp.set_header_content_type(file_dir);

        //[INFO] Get the file size
		server_resp.set_header_content_length(file);


		//[INFO] WRITE/SEND THE HEADERS
		std::cout << "SERVER: Sending GET response: \n" << std::endl;
		std::string response = server_resp.serialize_headers();
		//std::cout << "DEBUG send response:" << response << std::endl;
		server_resp.write_to_socket(response.c_str(), response.size());

		//[INFO] write/send the body of the response in chunks for speed
        const std::streamsize BUFSIZE = 8192;
        char buffer[BUFSIZE];
        std::streamsize n;
        while ((n = file.read(buffer, BUFSIZE).gcount()) > 0) {
			//std::cout << "DEBUG: writing body: " << std::string(buffer).substr(0, n) <<  std::endl;
            server_resp.write_to_socket(buffer, n);
        }

		//[INFO] END OF GET REQUEST
        file.close();
    } else {
        std::cout << "Error opening file 404 error" << std::endl;
        server_resp.set_status_code("404");
		return 404;
    }
    return 200;
}