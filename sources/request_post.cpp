#include "webserver.h"
#include "typedef.h"

#include <iostream>
#include <fstream>
#include <string.h>
#include <ctime>
#include <sstream>

void add_location_to_server(Connection &connection, std::string loc) {
	Location location;
	location.location = loc;
	location.autoindex = false;
	location.accepted_methods.push_back("GET");
	location.accepted_methods.push_back("DELETE");
	location.index = loc;
	connection._server.locations.push_back(location);
	//DEBUG ADD LOCATION
	connection._server.print_locations(connection._server.locations);
}

string get_time() {
	std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    std::stringstream ss;
	ss << (now->tm_year + 1900) << '-' 
        << (now->tm_mon + 1) << '-'
        <<  now->tm_mday << '_'
		<<  now->tm_hour << ':'
		<<  now->tm_min << ':'
		<<  now->tm_sec;
	string timestamp;
	ss >> timestamp;
	return timestamp;
}
//Not sure how to implement POST request jet. 
//For now it opens the file in URL and appends the body to it.
int post_request(Connection &connection) {
	std::string body = connection._request.get_body();
	std::string path_upload = connection._resp._location_serv->path_uploads;
	string root = connection._server.root;
	string time = get_time();
	std::vector<std::string> extention = connection._request.get_extention();
	string first_extention = "." + extention[0];
	string upload_file_name = "upload_" + time + first_extention;
	string upload_file_location = root + path_upload + upload_file_name;


	//in extentions we have all extentions poible fo that mime type, not using extention atm.

//	std::cout << "DEBUG: file_path_location: " << upload_file_location << std::endl;
//	std::cout << "DEBUG: body POST req: " << body << std::endl;
	std::ofstream file_receive;
	file_receive.open(upload_file_location.c_str(), std::ios::app);
	if (!file_receive.is_open()) {
		std::cerr << "Error open: " << strerror(errno) << std::endl;
			connection._resp.set_status_code("500");
		return (1);
	}
	file_receive << body;
	file_receive.close();

	//Add location to server for Delete to work
	string loc = path_upload + upload_file_name;
	add_location_to_server(connection, loc);

	//[INFO] SENDING RESPONSE
	ifstream file_send;
 	file_send.open(connection._resp._file_path.c_str(), std::ios::binary);

    if (file_send.is_open()) {

        //[INFO] Determine the MIME type of the file
        connection._resp.set_header_content_type(connection._resp._file_path);

        //[INFO] Get the file size
		connection._resp.set_header_content_length(file_send);

		//[INFO] WRITE/SEND THE HEADERS
		//std::cout << "SERVER: Sending POST response: \n" << std::endl;
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
        while ((n = file_send.read(buffer, BUFSIZE).gcount()) > 0) {
			//std::cout << "DEBUG: writing body: " << std::string(buffer).substr(0, n) <<  std::endl;
            ret = connection._resp.write_to_socket(buffer, n);
			if (ret == -1) {
				return 1;
		}
        }

		//[INFO] END OF GET REQUEST
        file_send.close();
    } 
	else {//Server side error
       //std::cout << "DEBUG: Error opening file 500 error" << std::endl; 
        connection._resp.set_status_code("500");
		error_request(connection);
    }
	return (0);
}