#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "typedef.h"
#include <string>
#include <vector>
#include <map>

class Location {
public:
	std::string 				location;
	bool 						autoindex;
	std::vector<std::string>	accepted_methods;
	std::string 				index;
	std::string					default_file;
	std::string 				cgi_path;
	std::string					path_uploads;
};

class ConfigServer {
private:

	const std::vector<std::string>	_init_keywords();
	template <typename T>
	void							_parse_number(T &dst, std::string::iterator it);
	void							_parse_string(std::string &dst, std::string::iterator it);
	void							_parse_location_value(std::string &dst, std::string::iterator it);
	void							_parse_error_pages(std::map<int, std::string> &dst, std::string::iterator it);
	void							_parse_bool(bool &dst, std::string::iterator it);
	void							_parse_methods(std::vector<std::string> &dst, std::string::iterator it);
	void							_parse_location(std::string &dst, std::string::iterator &it);
	void							_parse_redirect(std::vector<Location>	&dst, std::string::iterator &it, std::vector<std::string> keywords);
	void							_next_directive(std::string::iterator &it);

public:
	const std::vector<std::string>	keywords;


	int 							listen_port;
	std::string 					root;
	std::string 					server_name;
	std::map<int, std::string>		error_pages;
	long							size_content;
	//std::string						redir_src;
	//std::string						redir_dst;
	std::vector<Location>			locations;
	int								server_soc; //special case, meybe need to be 
	//somewhere else outside this class

	ConfigServer();
	ConfigServer(ConfigServer const &other);
	ConfigServer &operator=(ConfigServer const &other);
	~ConfigServer();

	int		parse_keyword(std::string::iterator &it);
	int		cmp_directive(std::string::iterator it, std::string directive);
	bool	case_ins_strcmp(const std::string s1, const std::string s2);
	void	print_locations(std::vector<Location> locs);
	bool	all_num(std::string str);

	void	check_req_direcs();

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
	class UnknownKeyword: public std::exception
	{
		public:
			virtual const char *	what() const throw()
			{
				return ("This keyword is unknown");
			}
	};
	class NoValueFound: public std::exception
	{
		public:
			const char *	what() const throw()
			{
				return ("No value found after keyword.");
			}
	};
	class IncorrectErrorPage: public std::exception
	{
		public:
			const char *	what() const throw()
			{
				return ("This error page is incorrect. Usage: error_page <error code> <path/to/errorpage>");
			}
	};
	class IncorrectLocationBlock: public std::exception
	{
		public:
			const char *	what() const throw()
			{
				return ("Location directive has wrong format.");
			}
	};
	class IncorrectValue: public std::exception
	{
		public:
			const char *	what() const throw()
			{
				return ("The value given is incorrect.");
			}
	};
	class ExpectedSemicolon: public std::exception
	{
		public:
			const char *	what() const throw()
			{
				return ("Expected semicolon after value.");
			}
	};
	class WrongListenPort: public std::exception
	{
		public:
			const char *	what() const throw()
			{
				return ("The directive listen_port must be set and can't be negative.");
			}
	};
	class NoRoot: public std::exception
	{
		public:
			const char *	what() const throw()
			{
				return ("The directive root must be set.");
			}
	};
	class WrongSizeContent: public std::exception
	{
		public:
			const char *	what() const throw()
			{
				return ("The directive client_max_body_size must be set and can't be 0 or negative.");
			}
	};
	class ValueMustBeNumber: public std::exception
	{
		public:
			const char *	what() const throw()
			{
				return ("The value of the listen and client_max_body_size must be an integer.");
			}
	};
};

int			parse_config(std::string config, std::vector<ConfigServer> &servers);
int			check_brackets(std::string config);
int			skipspace(std::string::iterator it);
void		print_servers(std::vector<ConfigServer> servers);
std::string	it_to_str(std::string::iterator it);

enum	token_types
{
	LISTEN,
	ROOT,
	SERVER_NAME,
	ERROR_PAGE,
	CLIENT_BODY_SIZE,
	REDIRECTION,
	INDEX,
	AUTOINDEX,
	METHODS,
	DEFAULT_FILE,
	CGI,
	PATH_UPLOADS
};

#endif
