#include "webserver.h"
#include "typedef.h"
#include <cstdio>

int delete_request(Connection &connection) {
	std::string file_dir;
	file_dir = connection._resp._file_path;
	cout << "SERVER: Deleting file: " << file_dir << endl;
	if (remove(file_dir.c_str()) != 0) {
		std::cout << "DEBUG: Error deleting file 500 error" << std::endl;
		connection._resp.set_status_code("500");
		error_request(connection);
	}
	return 0;
}
