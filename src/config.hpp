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
	std::vector<std::string>	tokens;

	int check_brackets(std::string config);
	int	parse_config(std::string config, ConfigServer data);
	int	skipspace(std::string::iterator it);
	int	parse_keyword(std::string::iterator it, ConfigServer data);
	int	ConfigServer::cmp_token(std::string::iterator it, std::vector<std::string> tokens);

	ConfigServer();

	class NoBracketAferServer: public std::exception
		{
			public:
				const char *	what() const throw()
				{
					return ("no bracket near server keyword.");
				}
		};
	class UnbalancedBrackets: public std::exception
		{
			public:
				const char *	what() const throw()
				{
					return ("The brackets of the config file are unbalanced.");
				}
		};
	// int check_brackets(std::string	config, std::string::iterator it);
};

enum	token_types
{
	LISTEN,
	ROOT,
	CGI,
	SERVER_NAME
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
