#include "../includes/webserver.h"
#include "Request.hpp"
#include "Response.hpp"
#include <iostream>
#include <fstream>

//Not sure how to implement POST request jet. 
//For now it opens the file in URL and appends the body to it.
int post_request(Request &client_request, Response &resp) {
	std::string body = client_request.get_body();
	std::string path = client_request.get_path();
	//in extentions we have all extentions poible fo that mime type, not using extention atm.
	std::vector<std::string> extention = client_request.get_extention();

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

	resp.set_status_code("200");

	return (0);
}