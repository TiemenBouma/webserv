#include "webserver.h"
#include "typedef.h"
#include <fstream>

#include <unistd.h>


int get_request(Connection &connection) {
    std::ifstream file;
    std::string file_dir;

    //[INFO] Open the file in binary mode
	//cout << "DEBUG: open page: " << connection._resp._file_path << endl;

    file.open(connection._resp._file_path.c_str(), std::ios::binary);

    if (file.is_open()) {

        //[INFO] Determine the MIME type of the file
        connection._resp.set_header_content_type(connection._resp._file_path);

        //[INFO] Get the file size
		connection._resp.set_header_content_length(file);

		//[INFO] WRITE/SEND THE HEADERS
		//std::cout << "SERVER: Sending GET response: \n" << std::endl;
		std::string response_string = connection._resp.serialize_headers();
		//std::cout << "DEBUG send response:\n" << response_string << std::endl;
		ssize_t ret = connection._resp.write_to_socket(response_string.c_str(), response_string.size());
		if (ret == -1) {
			return 1;
		}

		//[INFO] write/send the body of the response in chunks for speed
        const std::streamsize BUFSIZE = BUFFER_SIZE_8K;
        char buffer[BUFSIZE];
        std::streamsize n;
        while ((n = file.read(buffer, BUFFER_SIZE_8K).gcount()) > 0) {
			//std::cout << "DEBUG: writing body: " << std::string(buffer).substr(0, n) <<  std::endl;
            ret = connection._resp.write_to_socket(buffer, n);
			if (ret == -1) {
				return 1;
		}
        }

		//[INFO] END OF GET REQUEST
        file.close();
    } 
	else {//Server side error
        //std::cout << "DEBUG: Error opening file 500 error" << std::endl; 
        connection._resp.set_status_code("500");
		error_request(connection);
    }
    return 0;
}
