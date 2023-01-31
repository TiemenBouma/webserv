#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>
#include <map>

typedef struct s_location {
	std::string location;
	bool 		autoindex;
	std::vector<std::string>	unaccepted_methods;
	std::string 				index;
} t_location;

class ConfigServer {
public:
	int 						listen_port;
	std::string 				root;
	std::string 				server_name;
	std::map<int, std::string>	error_pages;
	long						size_content;
	std::string 				cgi_path;
	std::string					redir_src;
	std::string					redir_dst;
	t_location					loc;

	int check_brackets(std::string	config);
	int	parse_config(std::string	config);
	// int check_brackets(std::string	config, std::string::iterator it);
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
