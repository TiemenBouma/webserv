#include "../includes/webserver.h"

int execute_request(Request &req, Response &resp) {
	if (req.get_method() == "GET")
		get_request(req, resp);
	// else if (req.get_method() == "POST")

	// else if (req.get_method() == "DELETE")

	// else
	// 	error;
	return 0;
}