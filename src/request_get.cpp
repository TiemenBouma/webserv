#include "webserver.h"
#include "typedef.h"
#include <fstream>


#include <unistd.h>


int get_request(Connection &connection) {
    std::ifstream file;
    std::string file_dir;
	//[INFO] DETERMINE THE FILE TO OPEN
    if (connection._request.get_path() == "/") {
        file_dir = connection._server.root + connection._server.locations[2].index;
		std::cout << "[DEBUG] filedir = " << file_dir << std::endl;
    } else {
        file_dir = "../data" + connection._request.get_path();
    }

    //[INFO] Open the file in binary mode
    file.open(file_dir.c_str(), std::ios::binary);



    if (file.is_open()) {

        //[INFO] Determine the MIME type of the file
        connection._resp.set_header_content_type(file_dir);


        //[INFO] Get the file size
		connection._resp.set_header_content_length(file);



		//[INFO] WRITE/SEND THE HEADERS
		std::cout << "SERVER: Sending GET response: \n" << std::endl;
		std::string response = connection._resp.serialize_headers();
		//std::cout << "DEBUG send response:" << response << std::endl;
		connection._resp.write_to_socket(response.c_str(), response.size());

		//[INFO] write/send the body of the response in chunks for speed
        const std::streamsize BUFSIZE = 8192;
        char buffer[BUFSIZE];
        std::streamsize n;
        while ((n = file.read(buffer, BUFSIZE).gcount()) > 0) {
			//std::cout << "DEBUG: writing body: " << std::string(buffer).substr(0, n) <<  std::endl;
            connection._resp.write_to_socket(buffer, n);

        }

		//[INFO] END OF GET REQUEST
        file.close();
    } else {
        std::cout << "Error opening file 404 error" << std::endl;
        connection._resp.set_status_code("404");
		return 404;
    }
    return 200;
}