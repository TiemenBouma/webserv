#ifndef CONFIG_HPP
#define CONFIG_HPP

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
	bool		 				cgi;
	std::string					path_uploads;
	std::map<int, std::string>	redir;
};

class ConfigServer {
private:

	const std::vector<std::string>	_init_keywords();
	template <typename T>
	void							_parse_number(T &dst, std::string::iterator it);
	void							_parse_string(std::string &dst, std::string::iterator it);
	void							_parse_location_value(std::string &dst, std::string::iterator it);
	void							_parse_int_str_map(std::map<int, std::string> &dst, std::string::iterator it);
	void							_parse_bool(bool &dst, std::string::iterator it);
	void							_parse_methods(std::vector<std::string> &dst, std::string::iterator it);
	void							_parse_location(std::string &dst, std::string::iterator &it);
	void							_parse_loc_keyword(std::vector<Location>	&dst, std::string::iterator &it, std::vector<std::string> keywords);
	void							_next_directive(std::string::iterator &it);

public:
	const std::vector<std::string>	keywords;


	int 							listen_port;
	std::string 					root;
	std::string 					server_name;
	std::map<int, std::string>		error_pages;
	long							size_content;
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
	void	check_double_ports(std::vector<ConfigServer> servers);

	class NoBracketAfterServer: public std::exception
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
	class IncorrectMapFormat: public std::exception
	{
		public:
			const char *	what() const throw()
			{
				return ("Incorrect map directive format. Usage: error_page/redirection <error code;redir code> <path/to/errorpage;redir location>");
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
	class DoubleDirective: public std::exception
	{
		public:
			const char *	what() const throw()
			{
				return ("A directive cannot be specified twice. Except for error_pages");
			}
	};
	class IdenticalPortNumbers: public std::exception
	{
		public:
			const char *	what() const throw()
			{
				return ("Some servers have the same port numbers.");
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
	LISTEN				= 0x1,
	ROOT				= 0x2,
	SERVER_NAME			= 0x4,
	ERROR_PAGE			= 0x8,
	CLIENT_BODY_SIZE	= 0x10,
	LOCATION			= 0x20,
	INDEX				= 0x40,
	AUTOINDEX			= 0x80,
	METHODS				= 0x100,
	DEFAULT_FILE		= 0x200,
	CGI					= 0x400,
	PATH_UPLOADS		= 0x800,
	REDIRECTION			= 0x1000
};

#endif
