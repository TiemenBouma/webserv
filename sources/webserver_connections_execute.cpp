/**
 * @file webserver_connections_execute.cpp
 * @author tbauma
 * 
 * @brief webserver 42 network
 * @version 0.2
 * 
 * @date 2023-03-29
 */

#include "webserver.h"
#include "Cgi.hpp"
#include <dirent.h>     // DIR
#include <string>	   // string
#include <vector>	   // vector


string autoindex_response(string &directory_path, vector<string> &dir_content) {
    string response;
    response += "<html>\n";
    response += "<head><title>Index of " + directory_path + "</title></head>\n";
    response += "<body>\n";
    response += "<h1>Index of " + directory_path + "</h1>\n";
    response += "<hr>\n";
    response += "<pre>\n";
    for (vector<string>::const_iterator it = dir_content.begin(); it != dir_content.end(); ++it) 
    {
        const string& filename = *it;
        if (filename == "." || filename == "..")
            continue;
        response += "<a href=\"" + filename + "\">" + filename + "</a>\n";
    }
    response += "</pre>\n";
    response += "<hr>\n";
    response += "</body>\n";
    response += "</html>\n";
    return(response);
}

int autoindex_get(Connection &connection, string &dir) {

    DIR* dirp = opendir(dir.c_str());
    if (dirp == NULL) 
    {
        cerr << "Error: " << dir << " is not a directory.\n";
        return 1;
    }

    struct dirent* dp;
	vector<string> dir_content;
    while ((dp = readdir(dirp)) != NULL) {
		 dir_content.push_back(dp->d_name);
	}
    string response = autoindex_response(dir, dir_content); 
    closedir(dirp);

	string html = "file.html";
	connection._response.set_header_content_type(html);
	connection._response.set_header_content_length_string(response);

	std::string headers = connection._response.serialize_headers();

	ssize_t ret = connection._response.write_to_socket(headers.c_str(), headers.size());
	if (ret == -1) {
		return 1;
	}
	ret = connection._response.write_to_socket(response.c_str(), response.size());
	if (ret == -1) {
		return 1;
	}
    return 0;
}

int execute_request(Connection &connection) {

	//[INFO] Preparing response, error checking
	connection._response.set_client_socket(connection._socket);
	connection.set_location();
	if (connection._response._status_code != "200") {
		error_request(connection);
		return 1;
	}
	connection.check_method();
	if (connection._response._status_code != "200") {
		error_request(connection);
		return 1;
	}

	if (connection._request.get_method() == "GET") {
		if (connection._response._location_server->autoindex == 1) {
			string index_location = connection._server.root + connection._response._location_server->location;
			autoindex_get(connection, index_location);
		}
		if (connection._response._location_server->cgi == 1)
			cgi_get_request(connection);
		else
			get_request(connection);
	}
	else if (connection._request.get_method() == "POST") {
		if (connection._response._location_server->cgi == 1)
			cgi_post_request(connection);
		else
			post_request(connection);
	}
	else if (connection._request.get_method() == "DELETE") {
		delete_request(connection);
	}
	return 0;
}
