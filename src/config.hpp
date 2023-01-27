#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <map>

#define MIME_TYPES_FILE "../data/MIME_TYPES.txt"
#define HOMEPAGE_FILE "../data/homepage.html"
// #define ROOT_DIR "../data/"
// const int PORT = 8080;

typedef struct s_location {
	std::string location;
	bool 		autoindex;
	std::vector<std::string>	unaccepted_methods;
	std::string 				index;
} t_location;

class ConfigFile {
public:
	int 						port;
	std::string 				root;
	std::string 				server_name;
	std::map<int, std::string>	error_pages;
	long						size_content;
	std::string 				cgi_path;
	std::string					redir_src;
	std::string					redir_dst;
};

// typedef struct s_server {
// 	int 						listen_port;
// 	std::string 				root;
// 	std::string 				cgi_path;
// 	std::string 				name;
// 	std::vector<t_location> 	locations;
// 	std::map<int, std::string>	error_pages;
// 	std::string 				error_page;
// 	int							error_number;
// 	long						size_string;
// 	std::string					redir_src;
// 	std::string					redir_dst;
// } t_server;

// typedef struct s_settings {
// 	std::vector<t_server> 		servers;
// } t_settings;

#endif
