#include "Request.hpp"
#include "Response.hpp"
#include <fstream>
#include <string>
#include <iostream>

int get_request(Request &req, Response &resp) {
		// Open the file in input mode
	//std::string main_dir = ;
	//std::string url = ;

	std::ifstream file;
	std::string data;

	if (req.get_url() == "/")
		file.open("../homepage.html");
	else {
		std::string file_dir = ".." + req.get_url();
		file.open(file_dir.c_str());
		std::cout << "DEBUG: " << file_dir << std::endl;
	}

	if (file.is_open()) {
		char buf[2000000];
		while (1) {
			file.read((char *)buf, 2000000);
			data += buf;
			if(file.eof())
				break;
		}
		resp.set_body(data);
		file.close();
	} else {
		std::cout << "Error opening file 404 error" << std::endl;
	}
	return 0;
}