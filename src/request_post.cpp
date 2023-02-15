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
         <<  now->tm_mday << ' '
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
	std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
	//in extentions we have all extentions poible fo that mime type, not using extention atm.
	std::vector<std::string> extention = connection._request.get_extention();

	std::string file_path = ROOT_DIR + path;
	std::cout << "DEBUG: file_path: " << file_path << std::endl;
	std::cout << "DEBUG: body POST req: " << body << std::endl;
	std::ofstream file;
	file.open(file_path.c_str(), std::ios::app);
	if (!file.is_open()) {
		std::cerr << "Error open: " << strerror(errno) << std::endl;
		return (1);
	}
	file << body;
	file.close();

	connection._resp.set_status_code("200");

	return (0);
}