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
	std::string file_dir;

	if (req.get_url() == "/") {
		file_dir = "../homepage.html";
		file.open(file_dir.c_str());
	}
	else {
		file_dir = ".." + req.get_url();
		file.open(file_dir.c_str());
	}

	if (file.is_open()) {
		resp.set_header_content_type(file_dir);
		char buf[2000000];
		while (1) {
			file.read((char *)buf, 2000000);
			data += buf;
			if(file.eof())
				break;
		}
		resp.set_body(data);
		resp.set_header_content_length(data.length());
		file.close();
	} else {
		std::cout << "Error opening file 404 error" << std::endl;
	}
	return 0;
}