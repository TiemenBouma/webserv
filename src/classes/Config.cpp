#include "Config.hpp"
#include <iostream>
//#include "webserver.h"

ConfigServer::ConfigServer(ConfigServer const &other) {
	*this = other;
}

ConfigServer &ConfigServer::operator=(ConfigServer const &other) {
	if (this != &other) {
		this->listen_port = other.listen_port;
		this->root = other.root;
		this->server_name = other.server_name;
		this->error_pages = other.error_pages;
		this->size_content = other.size_content;
		this->redir_src = other.redir_src;
		this->redir_dst = other.redir_dst;
		this->locations = other.locations;
		this->keywords = other.keywords;
	}
	return *this;
}
ConfigServer::~ConfigServer(){}





/* 
	PRIVATE
*/

const std::vector<std::string>	ConfigServer::_init_keywords()
{
	std::vector<std::string> ret;
	ret.push_back("listen");
	ret.push_back("root");
	ret.push_back("server_name");
	ret.push_back("error_page");
	ret.push_back("client_max_body_size");
	ret.push_back("location");
	ret.push_back("index");
	ret.push_back("autoindex");
	ret.push_back("allowed_methods");
	ret.push_back("default_file");
	ret.push_back("cgi_path");
	ret.push_back("path_uploads");
	ret.push_back("invalid");
	return (ret);
}

template <typename T>
void	ConfigServer::_parse_number(T &dst, std::string::iterator it)
{
	std::string	value = "";

	while (*it != ' ' && *it != '\t' && *it != ';' && *it != '}')
		it++;
	while (*it == '\t' || *it == ' ')
		it++;
	if (*it == '\n' || *it == ';' || *it == '}')
		throw(NoValueFound());
	for (; isspace(*it) == 0 && *it != ';' && *it != '}'; it++)
		value += *it;
	if (*it != ';')
		throw(ExpectedSemicolon());
	dst = std::stoi(value);
}

void	ConfigServer::_parse_string(std::string &dst, std::string::iterator it)
{
	std::string	value = "";

	while (*it != ' ' && *it != '\t' && *it != ';' && *it != '}')
		it++;
	while ((*it == '\t' || *it == ' ') && *it != '}')
		it++;
	if (*it == '\n' || *it == ';')
		throw(NoValueFound());
	for (; isspace(*it) == 0 && *it != ';' && *it != '}'; it++)
		value += *it;
	dst = value;
	if (*it != ';')
		throw(ExpectedSemicolon());
}

void	ConfigServer::_parse_location_value(std::string &dst, std::string::iterator it)
{
	std::string	value = "";

	while (*it != ' ' && *it != '\t' && *it == '\n' && *it != ';' && *it != '{')
		it++;
	it += skipspace(it);
	if (*it == '{' || *it == ';')
		throw(NoValueFound());
	for (; *it != '\n' && *it != ';' && *it == '{' ; it++)
		value += *it;
	dst = value;
}

void	ConfigServer::_parse_error_pages(std::map<int, std::string> &dst, std::string::iterator it)
{
	std::string	value1 = "";
	std::string	value2 = "";

	while (*it != ' ' && *it != '\t' && *it != ';' && *it != '}')
		it++;
	while ((*it == '\t' || *it == ' ') && *it != '}')
		it++;
	if ((*it == '\n' || *it == ';') && *it == '}')
		throw(NoValueFound());
	if (std::isdigit(*it == 0))
		throw(IncorrectErrorPage());
	for (; std::isdigit(*it) != 0; it++)
		value1 += *it;
	if (value1.size() == 0)
		throw(IncorrectErrorPage());
	while (*it == '\t' || *it == ' ')
		it++;
	if (*it == '\n' || *it == ';' || *it == '}')
		throw(IncorrectErrorPage());
	for (; isspace(*it) == 0 && *it != ';' && *it != '}'; it++)
		value2 += *it;
	if (value1.size() == 0)
		throw(IncorrectErrorPage());
	if (*it != ';')
		throw(ExpectedSemicolon());
	dst.insert(std::pair<int, std::string>(stoi(value1), value2));
}

void	ConfigServer::_parse_bool(bool &dst, std::string::iterator it)
{
	std::string			tmp;

	_parse_string(tmp, it);
	if (tmp == "1" || case_ins_strcmp(tmp, "on"))
		dst = true;
	else if (tmp == "0" || case_ins_strcmp(tmp, "off"))
		dst = false;
	else
		throw(IncorrectValue());
}

void	ConfigServer::_parse_methods(std::vector<std::string> &dst, std::string::iterator it)
{
	while (*it != ' ' && *it != '\t' && *it != ';' && *it != '}')
		it++;
	while ((*it == '\t' || *it == ' ') && *it != '}')
		it++;
	if ((*it == '\n' || *it == ';') && *it == '}')
		throw(NoValueFound());
	while (*it != '\n' && *it != ';' && *it != '}')
	{
		while ((*it == '\t' || *it == ' ') && *it != '}')
			it++;
		if (*it == '\n' || *it == ';' || *it == '}')
			throw(NoValueFound());
		std::string	value = "";
		for (; *it != '\n' && *it != ';' && *it != ' ' && *it != '\0'; it++)
			value += *it;
		dst.push_back(value);
	}
	if (*it != ';')
		throw(ExpectedSemicolon());
}

void	ConfigServer::_parse_location(std::string &dst, std::string::iterator &it)
{
	std::string	value = "";

	while (isspace(*it) == 0 && *it != '{' && *it != '\0')
		it++;
	if (*it == '{' || *it == '\0')
		throw(IncorrectLocationBlock());
	it += skipspace(it);
	if (*it == '{' || *it == '\0')
		throw(IncorrectLocationBlock());
	for (; isspace(*it) == 0 && *it != '{' && *it != '\0'; it++)
		value += *it;
	if (*it == '{' || *it == '\0')
		throw(IncorrectLocationBlock());
	it += skipspace(it);
	if (*it == '{')
		it++;
	else
		throw(IncorrectLocationBlock());
	it += skipspace(it);
	dst = value;
}

void	ConfigServer::_parse_redirect(std::vector<t_location> &dst, std::string::iterator &it, std::vector<std::string> keywords)
{
	int	i;
	t_location	new_loc;

	new_loc.autoindex = 0;
	_parse_location(new_loc.location, it);
	std::cout << "\tin data class: " << new_loc.location << std::endl;
	std::cout << "\tparsed location" << std::endl;
	while (*it != '}' && *it != '\0')
	{
		i = 0;
		while (cmp_directive(it, keywords[i]) == 0)
			i++;
		std::cout << "\tin location block: i: " << i << ", directive found: " << keywords[i] << std::endl;
		switch (i)
		{
			case INDEX:
				_parse_string(new_loc.index, it);
				std::cout << "\tin data class: " << new_loc.index << std::endl;
				std::cout << "\tparsed index" << std::endl;
				break;
			case AUTOINDEX:
				_parse_bool(new_loc.autoindex, it);
				std::cout << "\tin data class: " << new_loc.autoindex << std::endl;
				std::cout << "\tparsed autoindex" << std::endl;
				break;
			case METHODS:
				_parse_methods(new_loc.accepted_methods, it);
				std::cout << "\tin data class: " << new_loc.accepted_methods[1] << std::endl;
				std::cout << "\tparsed methods" << std::endl;
				break;
			case DEFAULT_FILE:
				_parse_string(new_loc.default_file, it);
				std::cout << "\tin data class: " << new_loc.default_file << std::endl;
				std::cout << "\tparsed default file" << std::endl;
				break;
			case CGI:
				_parse_string(new_loc.cgi_path, it);
				std::cout << "\tin data class: " << new_loc.cgi_path << std::endl;
				std::cout << "\tparsed cgi path" << std::endl;
				break;
			case PATH_UPLOADS:
				_parse_string(new_loc.path_uploads, it);
				std::cout << "\tin data class: " << new_loc.path_uploads << std::endl;
				std::cout << "\tparsed path uploads" << std::endl;
				break;
			default:
				throw(UnknownKeyword());
		}
		_next_directive(it);
	}
	dst.push_back(new_loc);
}

void	ConfigServer::_next_directive(std::string::iterator &it)
{
	if (*it == '}')
	{
		it += 1;
		it += skipspace(it);
		return ;
	}
	while (*it != '}' && *it != '\n' && *it != '\0')
	{
		it++;
	}
	while (isspace(*it) != 0)
		it++;
}

/*
	PUBLIC
*/

ConfigServer::ConfigServer(): keywords(_init_keywords())
{
}

int	ConfigServer::parse_keyword(std::string::iterator &it)
{
	int	i;
	std::map<int, std::string>::iterator	mapit;

	while (*it != '}' && *it != '\0')
	{
		i = 0;
		while (cmp_directive(it, keywords[i]) == 0)
			i++;
		std::cout << "in server block: i: " << i << ", directive found: " << keywords[i] << std::endl;
		switch (i)
		{
			case LISTEN:
				_parse_number(listen_port, it);
				std::cout << "in data class: " << listen_port << std::endl;
				std::cout << "parsed listen" << std::endl;
				break;
			case ROOT:
				_parse_string(root, it);
				std::cout << "in data class: " << root << std::endl;
				std::cout << "parsed root" << std::endl;
				break;
			case SERVER_NAME:
				_parse_string(server_name, it);
				std::cout << "in data class: " << server_name << std::endl;
				std::cout << "parsed server name" << std::endl;
				break;
			case ERROR_PAGE:
				_parse_error_pages(error_pages, it);
				for(mapit = error_pages.begin(); mapit != error_pages.end(); mapit++)
					std::cout << "in error pages: " << mapit->first << ", " << mapit->second << std::endl;
				std::cout << "parsed error pages" << std::endl;
				break;
			case CLIENT_BODY_SIZE:
				_parse_number(size_content, it);
				std::cout << "in data class: " << size_content << std::endl;
				std::cout << "parsed client body size" << std::endl;
				break;
			case REDIRECTION:
				_parse_redirect(locations, it, keywords);
				print_locations(locations);
				std::cout << "parsed redirect" << std::endl;
				break;
			default:
				throw(UnknownKeyword());
		}
		std::cout << "*it before skipping: '" << *it << "'" << std::endl;
		_next_directive(it);
		std::cout << "*it after skipping: '" << *it << "'" << std::endl;
	}
	return (0);
}

int	ConfigServer::cmp_directive(std::string::iterator it, std::string directive)
{
	int	i = 0;

	if (directive == "invalid")
		return (1);
	while (*it == directive[i])
	{
		it++;
		i++;
	}
	if ((*it == '\t' || *it == ' ') && directive[i] == '\0')
		return (1);
	return (0);
}

bool	ConfigServer::case_ins_strcmp(const std::string s1, const std::string s2)
{
	int	i = 0;

	if (s1.size() != s2.size())
		return false;
	for (; i < static_cast<int>(s1.size()); i++)
	{
		if (std::tolower(s1[i]) != std::tolower(s2[i]))
			return false;
	}
	return (true);
}

void	ConfigServer::print_locations(std::vector<t_location> locs)
{
	for (std::vector<t_location>::iterator it = locs.begin(); it != locs.end(); it++)
	{
		std::cout << "in location block:" << std::endl;
		std::cout << "\tLocation: '" << (*it).location << "'" << std::endl;
		std::cout << "\tAutoindex: '" << (*it).autoindex << "'" << std::endl;
		std::cout << "\tAccepted methods:" << std::endl << "\t";
		for (std::vector<std::string>::iterator vit = (*it).accepted_methods.begin(); vit != (*it).accepted_methods.end(); vit++)
			std::cout << "'" << (*vit) << "' ";
		std::cout << std::endl;
		std::cout << "\tIndex: '" << (*it).index << "'" << std::endl;
		std::cout << "\tDefault file: '" << (*it).default_file << "'" << std::endl;
		std::cout << "\tCgi path: '" << (*it).cgi_path << "'" << std::endl;
		std::cout << "\tPath uploadss: '" << (*it).path_uploads << "'" << std::endl;
	}
}

/* 
	[INFO]FUNCTION OUTSIDE CLASS
*/

int	parse_config(std::string config, std::vector<ConfigServer> &servers)
{
	std::string::iterator	it = config.begin();

	if (check_brackets(config) == 0)
		throw(ConfigServer::UnbalancedBrackets());
	while (true)
	{
		ConfigServer	serv;

		if (*it == '}')
			it += 1;
		it += skipspace(it);
		if (*it == '\0')
			return (1);
		std::cout << "it pointing to: '" << *it << "'" << std::endl;
		if (serv.cmp_directive(it, "server") == 0)
			throw(ConfigServer::UnknownKeyword());
		it += strlen("server");
		it += skipspace(it);
		if (*it != '{')
			throw(ConfigServer::NoBracketAferServer());
		it += 1;
		it += skipspace(it);
		serv.parse_keyword(it);
		std::cout << std::endl << "parsed server" << std::endl << std::endl;
		servers.push_back(serv);
	}
	return (0);
}

int	check_brackets(std::string config)
{
	std::stack<char>	stack;
	int					brack_state = 0;

	//	[INFO]push all curly brackets to a stack
	for (std::string::iterator it = config.begin(); it != config.end(); it++)
	{
		if (*it == '{' || *it == '}')
			stack.push(*it);
	}
	std::cout << "stack size: " << stack.size() << std::endl;
	//	[INFO]check if the amount of left brackets are equal to the amount of right ones
	while (stack.size() > 0)
	{
		if (stack.top() == '}')
			brack_state++;
		if (stack.top() == '{')
			brack_state--;
		if (brack_state <= -1)
			return (0);
		stack.pop();
	}
	if (brack_state == 0)
		return (1);
	return (0);
}

int	skipspace(std::string::iterator it)
{
	int	ret = 0;
	while (isspace(*it) != 0)
	{
		it++;
		ret++;
	}
	return (ret);
}

void	print_servers(std::vector<ConfigServer> servers)
{
	for (std::vector<ConfigServer>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		std::cout << std::endl << "Server information:" << std::endl << std::endl;
		std::cout << "Listen: '" << (*it).listen_port << "'" << std::endl;
		std::cout << "Root: '" << (*it).root << "'" << std::endl;
		std::cout << "Server name: '" << (*it).server_name << "'" << std::endl;
		std::cout << "Error pages:" << std::endl;
		for (std::map<int, std::string>::iterator mapit = (*it).error_pages.begin(); mapit != (*it).error_pages.end(); mapit++)
			std::cout << "'" << (*mapit).first << ", " << (*mapit).second << "'" << std::endl;
		std::cout << "Size content: '" << (*it).size_content << "'" << std::endl;
		(*it).print_locations((*it).locations);
	}
}

