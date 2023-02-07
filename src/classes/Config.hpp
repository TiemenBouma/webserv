#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <map>

class Location {
	public:
	std::string					location;
	bool 						autoindex;
	std::vector<std::string>	unaccepted_methods;
	std::string 				index;
};

class ConfigServer {
public:
	//TO BE PARSED
	int 						listen_port;
	std::string					server_name;
	std::map<int, std::string>	error_pages;
	long						max_size_content;
	std::string					root;
	std::string					cgi_path;
	std::string					redir_src;
	std::string					redir_dst;
	std::vector<Location>		locations;
	//TO BE USED IN RUNTIME FOR WEBSERVER
	int							server_soc;
	// std::vector<int>			client_socs;

};



#endif