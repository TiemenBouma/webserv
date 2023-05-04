
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
		Location temp_location;
		temp_location.accepted_methods.push_back("GET");
		temp_location.autoindex = 0;
		temp_location.cgi = 0;
		temp_location.index = connection._response._location_server->location;
		temp_location.index += "/";
		temp_location.index += dp->d_name;

		temp_location.location = "/";
		temp_location.location += dp->d_name;
		connection._server.locations.push_back(temp_location);
		cout << "DEBUG: " << temp_location.index << endl;
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