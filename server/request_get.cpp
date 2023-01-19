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
		file.open(".." + req.get_url());

	if (file.is_open()) {
		if (htmp en text) {
			char * buf[2000000];
			while (getline(file, buf)) {
				resp.text += buf; + "\n";
			}
			file.close();

		}
		if (plaatje) {
			 ret = read(file, buff, 2000000)
			 if ret != 2000000)
			 	break ;
		}
	} else {
		std::cout << "Error opening file" << std::endl;
	}
	//std::cout << resp.text << std::endl;
	return 0;
}