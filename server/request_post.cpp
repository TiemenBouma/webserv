#include "../includes/webserver.h"
#include "Request.hpp"
#include "Response.hpp"
#include <iostream>
#include <fstream>

int post_request(Response &resp, Request &client_request) {
	std::string body = client_request.get_body();
	std::string path = client_request.get_path();
	std::string file_path = ROOT_DIR + path;
	std::cout << "DEBUG: file_path: " << file_path << std::endl;
	std::cout << "DEBUG: body: " << body << std::endl;
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
