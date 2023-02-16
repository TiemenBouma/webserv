#include "webserver.h"
#include "typedef.h"
// #include "../includes/webserver.h"

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>


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
	std::string path = connection._request.get_path();
	string time = get_time();
	std::vector<std::string> extention = connection._request.get_extention();
	string first_extention = "." + extention[0];
	string upload_file = path + time + first_extention;


	//in extentions we have all extentions poible fo that mime type, not using extention atm.

	std::cout << "DEBUG: file_path: " << upload_file << std::endl;
	std::cout << "DEBUG: body POST req: " << body << std::endl;
	std::ofstream file;
	file.open(upload_file.c_str(), std::ios::app);
	if (!file.is_open()) {
		std::cerr << "Error open: " << strerror(errno) << std::endl;
		return (1);
	}
	file << body;
	file.close();

	connection._resp.set_status_code("200");
	string response_str = connection._resp.serialize_headers();
	connection._resp.write_to_socket(response_str.c_str(), response_str.size());

	return (0);
}