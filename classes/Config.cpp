
#include <iostream>
#include <stack>
#include <sstream>
#include <cstring>

#include "Config.hpp"
#include "webserver.h"

ConfigServer::ConfigServer()
: keywords(_init_keywords()), listen_port(-1), size_content(-1)
{}

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
		//this->redir_src = other.redir_src;
		//this->redir_dst = other.redir_dst;
		this->locations = other.locations;
	}
	return *this;
}
ConfigServer::~ConfigServer(){}



/* 
	PRIVATE
*/

const vector<string>	ConfigServer::_init_keywords()
{
	vector<string> ret;
	ret.push_back("listen");
	ret.push_back("root");
	ret.push_back("server_name");
	ret.push_back("error_page");
	ret.push_back("client_max_body_size");
	ret.push_back("location");
	ret.push_back("index");
	ret.push_back("autoindex");
	ret.push_back("accepted_methods");
	ret.push_back("default_file");
	ret.push_back("cgi");
	ret.push_back("path_uploads");
	ret.push_back("invalid");
	return (ret);
}

// DEBUG Make sure this is a number
template <typename T>
void	ConfigServer::_parse_number(T &dst, string::iterator it)
{
	string	value = "";

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
	if (all_num(value) == false)
		throw(ValueMustBeNumber());
	stringstream ss(value);
	ss >> dst;
}

void	ConfigServer::_parse_string(string &dst, string::iterator it)
{
	string	value = "";

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

void	ConfigServer::_parse_location_value(string &dst, string::iterator it)
{
	string	value = "";

	while (*it != ' ' && *it != '\t' && *it == '\n' && *it != ';' && *it != '{')
		it++;
	it += skipspace(it);
	if (*it == '{' || *it == ';')
		throw(NoValueFound());
	for (; *it != '\n' && *it != ';' && *it == '{' ; it++)
		value += *it;
	dst = value;
}

void	ConfigServer::_parse_error_pages(map<int, string> &dst, string::iterator it)
{
	string	value1 = "";
	string	value2 = "";

	while (*it != ' ' && *it != '\t' && *it != ';' && *it != '}')
		it++;
	while ((*it == '\t' || *it == ' ') && *it != '}')
		it++;
	if ((*it == '\n' || *it == ';') && *it == '}')
		throw(NoValueFound());
	if (isdigit(*it) == 0)
		throw(IncorrectErrorPage());
	for (; isdigit(*it) != 0; it++)
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
	stringstream ss(value1);
	int i;
	ss >> i;
	dst.insert(pair<int, string>(i, value2));
}

void	ConfigServer::_parse_bool(bool &dst, string::iterator it)
{
	string			tmp;

	_parse_string(tmp, it);
	if (tmp == "1" || case_ins_strcmp(tmp, "on"))
		dst = true;
	else if (tmp == "0" || case_ins_strcmp(tmp, "off"))
		dst = false;
	else
		throw(IncorrectValue());
}

void	ConfigServer::_parse_methods(vector<string> &dst, string::iterator it)
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
		string	value = "";
		for (; *it != '\n' && *it != ';' && *it != ' ' && *it != '\0'; it++)
			value += *it;
		dst.push_back(value);
	}
	if (*it != ';')
		throw(ExpectedSemicolon());
}

void	ConfigServer::_parse_location(string &dst, string::iterator &it)
{
	string	value = "";

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

void	ConfigServer::_parse_redirect(vector<Location> &dst, string::iterator &it, vector<string> keywords)
{
	int	i;
	Location	new_loc;

	new_loc.cgi = 0;
	new_loc.autoindex = 0;
	_parse_location(new_loc.location, it);
	//cout << "\tin data class: " << new_loc.location << endl;
	//cout << "\tparsed location" << endl;
	while (*it != '}' && *it != '\0')
	{
		i = 0;
		while (cmp_directive(it, keywords[i]) == 0)
			i++;
	//	cout << "\tin location block: i: " << i << ", directive found: " << keywords[i] << endl;
		switch (i)
		{
			case INDEX:
				_parse_string(new_loc.index, it);
		//		cout << "\tin data class: " << new_loc.index << endl;
		//		cout << "\tparsed index" << endl;
				break;
			case AUTOINDEX:
				_parse_bool(new_loc.autoindex, it);
		//		cout << "\tin data class: " << new_loc.autoindex << endl;
		//		cout << "\tparsed autoindex" << endl;
				break;
			case METHODS:
				_parse_methods(new_loc.accepted_methods, it);
		//		cout << "\tin data class: " << new_loc.accepted_methods[1] << endl;
		//		cout << "\tparsed methods" << endl;
				break;
			case DEFAULT_FILE:
				_parse_string(new_loc.default_file, it);
		//		cout << "\tin data class: " << new_loc.default_file << endl;
		//		cout << "\tparsed default file" << endl;
				break;
			case CGI:
				_parse_bool(new_loc.cgi, it);
				// std::cout << "\tin data class: " << new_loc.cgi << std::endl;
				// std::cout << "\tparsed cgi path" << std::endl;
				break;
			case PATH_UPLOADS:
				_parse_string(new_loc.path_uploads, it);
		//		cout << "\tin data class: " << new_loc.path_uploads << endl;

		//		cout << "\tparsed path uploads" << endl;
				break;
			default:
				cout << "'" << it_to_str(it) << "'" << endl;
				throw(UnknownKeyword());
		}
		_next_directive(it);
	}
	dst.push_back(new_loc);
}

void	ConfigServer::_next_directive(string::iterator &it)
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

int	ConfigServer::parse_keyword(string::iterator &it)
{
	int	i;
	map<int, string>::iterator	mapit;

	while (*it != '}' && *it != '\0')
	{
		i = 0;
		while (cmp_directive(it, keywords[i]) == 0)
			i++;
		//cout << "in server block: i: " << i << ", directive found: " << keywords[i] << endl;
		switch (i)
		{
			case LISTEN:
				_parse_number(listen_port, it);
		//		cout << "in data class: " << listen_port << endl;
		//		cout << "parsed listen" << endl;
				break;
			case ROOT:
				_parse_string(root, it);
		//		cout << "in data class: " << root << endl;
		//		cout << "parsed root" << endl;
				break;
			case SERVER_NAME:
				_parse_string(server_name, it);
		//		cout << "in data class: " << server_name << endl;
		//		cout << "parsed server name" << endl;
				break;
			case ERROR_PAGE:
				_parse_error_pages(error_pages, it);
		//		for(mapit = error_pages.begin(); mapit != error_pages.end(); mapit++)
		//			cout << "in error pages: " << mapit->first << ", " << mapit->second << endl;
		//		cout << "parsed error pages" << endl;
				break;
			case CLIENT_BODY_SIZE:
				_parse_number(size_content, it);
		//		cout << "in data class: " << size_content << endl;
		//		cout << "parsed client body size" << endl;
				break;
			case REDIRECTION:
				_parse_redirect(locations, it, keywords);
		//		prinLocations(locations);
		//		cout << "parsed redirect" << endl;
				break;
			default:
				cout << "'" << it_to_str(it) << "'" << endl;
				throw(UnknownKeyword());
		}
	//	cout << "*it before skipping: '" << *it << "'" << endl;
		_next_directive(it);
	//	cout << "*it after skipping: '" << *it << "'" << endl;
	}
	return (0);
}

int	ConfigServer::cmp_directive(string::iterator it, string directive)
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

bool	ConfigServer::case_ins_strcmp(const string s1, const string s2)
{
	int	i = 0;

	if (s1.size() != s2.size())
		return false;
	for (; i < static_cast<int>(s1.size()); i++)
	{
		if (tolower(s1[i]) != tolower(s2[i]))
			return false;
	}
	return (true);
}

bool	ConfigServer::all_num(string str)
{
	for (string::iterator it = str.begin(); it != str.end(); it++)
	{
		if (isdigit(*it) == false)
			return (false);
	}
	return (true);
}

void	ConfigServer::check_req_direcs()
{
	if (listen_port < 0)
		throw(WrongListenPort());
	if (root == "")
		throw(NoRoot());
	// [CHECK] keeping these error pages bellow ?
	if (error_pages.find(400) == error_pages.end())
		error_pages.insert(pair<int, string>(400, "utils/default_error_pages/400bad_request.html"));
	if (error_pages.find(404) == error_pages.end())
		error_pages.insert(pair<int, string>(404, "utils/default_error_pages/404not_found.html"));
	if (error_pages.find(405) == error_pages.end())
		error_pages.insert(pair<int, string>(405, "utils/default_error_pages/405method_not_allowed.html"));
	if (error_pages.find(431) == error_pages.end())
		error_pages.insert(pair<int, string>(431, "utils/default_error_pages/431request_header_fields_too_large.html"));
	if (error_pages.find(500) == error_pages.end())
		error_pages.insert(pair<int, string>(500, "utils/default_error_pages/500internal_error.html"));

	if (size_content <= 0)
		throw(WrongSizeContent());
}

void	ConfigServer::print_locations(vector<Location> locs)
{
	for (vector<Location>::iterator it = locs.begin(); it != locs.end(); it++)
	{
		cout << GREEN << endl;
		cout << "\tLocation: '" << (*it).location << "'" << RESET << endl;
		cout << "\tAccepted methods:" << "\t";
		for (vector<string>::iterator vit = (*it).accepted_methods.begin(); vit != (*it).accepted_methods.end(); vit++)
			cout << "'" << (*vit) << "' ";
		cout << endl;
		cout << "\tIndex: '" << (*it).index << "'" << endl;
		cout << "\tPath uploadss: '" << (*it).path_uploads << "'" << endl;
		cout << "\tDefault file: '" << (*it).default_file << "'" << endl;
		cout << "\tAutoindex: '" << (*it).autoindex << "'" << endl;
		cout << "\tCgi: '" << (*it).cgi << "'" << endl;
	}
}
