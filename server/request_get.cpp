#include "Request.hpp"
#include "Response.hpp"
#include <fstream>
#include <string>
#include <iostream>

int get_request(Request &req, Response &resp) {
		// Open the file in input mode
	std::ifstream file;
	if (req.get_url() == "/")
		file.open("../homepage.html");
	else
		file.open(req.get_url());

	if (file.is_open()) {
		std::string buf;
		while (getline(file, buf)) {
			resp.text += buf + "\n";
		}
		file.close();
	} else {
		std::cout << "Error opening file" << std::endl;
	}
	//std::cout << resp.text << std::endl;
	return 0;
}