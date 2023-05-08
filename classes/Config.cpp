
#include <iostream>
#include <stack>
#include <sstream>
#include <cstring>
#include <cmath>

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
		this->locations = other.locations;
	}
	return *this;
}
ConfigServer::~ConfigServer(){}



/* 
	PRIVATE
*/


//[INFO] The order of this vector must match with the enum token_types in Config.hpp.
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
	ret.push_back("redirection");
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
	{
		std::cout << "[DEBUG] found semicolon error in _parse_string." << std::endl;
		throw(ExpectedSemicolon());
	}
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

void	ConfigServer::_parse_int_str_map(map<int, string> &dst, string::iterator it)
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
		throw(IncorrectMapFormat());
	for (; isdigit(*it) != 0; it++)
		value1 += *it;
	if (value1.size() == 0)
		throw(IncorrectMapFormat());
	while (*it == '\t' || *it == ' ')
		it++;
	if (*it == '\n' || *it == ';' || *it == '}')
		throw(IncorrectMapFormat());
	for (; isspace(*it) == 0 && *it != ';' && *it != '}'; it++)
		value2 += *it;
	if (value1.size() == 0)
		throw(IncorrectMapFormat());
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

void	ConfigServer::_parse_loc_keyword(vector<Location> &dst, string::iterator &it, vector<string> keywords)
{
	int			i;
	Location	new_loc;
	int			double_direc_check = 0;

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
		switch (static_cast<int>(pow(2, i)))
		{
			case INDEX:
				if ((double_direc_check & INDEX) == INDEX)
					throw(DoubleDirective());
				_parse_string(new_loc.index, it);
				double_direc_check |= INDEX;
		//		cout << "\tin data class: " << new_loc.index << endl;
		//		cout << "\tparsed index" << endl;
				break;
			case AUTOINDEX:
				if ((double_direc_check & AUTOINDEX) == AUTOINDEX)
					throw(DoubleDirective());
				_parse_bool(new_loc.autoindex, it);
				double_direc_check |= AUTOINDEX;
		//		cout << "\tin data class: " << new_loc.autoindex << endl;
		//		cout << "\tparsed autoindex" << endl;
				break;
			case METHODS:
				if ((double_direc_check & METHODS) == METHODS)
					throw(DoubleDirective());
				_parse_methods(new_loc.accepted_methods, it);
				double_direc_check |= METHODS;
		//		cout << "\tin data class: " << new_loc.accepted_methods[1] << endl;
		//		cout << "\tparsed methods" << endl;
				break;
			case DEFAULT_FILE:
				if ((double_direc_check & DEFAULT_FILE) == DEFAULT_FILE)
					throw(DoubleDirective());
				_parse_string(new_loc.default_file, it);
				double_direc_check |= DEFAULT_FILE;
		//		cout << "\tin data class: " << new_loc.default_file << endl;
		//		cout << "\tparsed default file" << endl;
				break;
			case CGI:
				if ((double_direc_check & CGI) == CGI)
					throw(DoubleDirective());
				_parse_bool(new_loc.cgi, it);
				double_direc_check |= CGI;
				// std::cout << "\tin data class: " << new_loc.cgi << std::endl;
				// std::cout << "\tparsed cgi path" << std::endl;
				break;
			case PATH_UPLOADS:
				if ((double_direc_check & PATH_UPLOADS) == PATH_UPLOADS)
					throw(DoubleDirective());
				_parse_string(new_loc.path_uploads, it);
				double_direc_check |= PATH_UPLOADS;
		//		cout << "\tin data class: " << new_loc.path_uploads << endl;

		//		cout << "\tparsed path uploads" << endl;
				break;
			case REDIRECTION:
				if ((double_direc_check & REDIRECTION) == REDIRECTION)
					throw(DoubleDirective());
				_parse_int_str_map(new_loc.redir, it);
				double_direc_check |= REDIRECTION;
		//		for(mapit = error_pages.begin(); mapit != error_pages.end(); mapit++)
		//			cout << "in error pages: " << mapit->first << ", " << mapit->second << endl;
		//		cout << "parsed error pages" << endl;
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
	int			double_direc_check = 0;

	while (*it != '}' && *it != '\0')
	{
		i = 0;
		while (cmp_directive(it, keywords[i]) == 0)
			i++;
		//cout << "in server block: i: " << i << ", directive found: " << keywords[i] << endl;
		switch (static_cast<int>(pow(2, i)))
		{
			case LISTEN:
				if ((double_direc_check & LISTEN) == LISTEN)
					throw(DoubleDirective());
				_parse_number(listen_port, it);
				double_direc_check |= LISTEN;
				// cout << "in data class: " << listen_port << endl;
				// cout << "parsed listen" << endl;
				break;
			case ROOT:
				if ((double_direc_check & ROOT) == ROOT)
					throw(DoubleDirective());
				_parse_string(root, it);
				double_direc_check |= ROOT;
		//		cout << "in data class: " << root << endl;
		//		cout << "parsed root" << endl;
				break;
			case SERVER_NAME:
				if ((double_direc_check & SERVER_NAME) == SERVER_NAME)
					throw(DoubleDirective());
				_parse_string(server_name, it);
				double_direc_check |= SERVER_NAME;
		//		cout << "in data class: " << server_name << endl;
		//		cout << "parsed server name" << endl;
				break;
			case ERROR_PAGE:
				_parse_int_str_map(error_pages, it);
		//		for(mapit = error_pages.begin(); mapit != error_pages.end(); mapit++)
		//			cout << "in error pages: " << mapit->first << ", " << mapit->second << endl;
		//		cout << "parsed error pages" << endl;
				break;
			case CLIENT_BODY_SIZE:
				if ((double_direc_check & CLIENT_BODY_SIZE) == CLIENT_BODY_SIZE)
					throw(DoubleDirective());
				_parse_number(size_content, it);
				double_direc_check |= CLIENT_BODY_SIZE;
		//		cout << "in data class: " << size_content << endl;
		//		cout << "parsed client body size" << endl;
				break;
			case LOCATION:
				_parse_loc_keyword(locations, it, keywords);
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
		cout << "\tRedirection: ";
		for (std::map<int, std::string>::iterator mapit = (*it).redir.begin(); mapit != (*it).redir.end(); mapit++)
			std::cout << "'" << (*mapit).first << ", " << (*mapit).second << "'" << std::endl;
	}
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

void	ConfigServer::check_double_ports(std::vector<ConfigServer> servers)
{
	std::vector<ConfigServer>::iterator it2;

	for (std::vector<ConfigServer>::iterator it1 = servers.begin(); it1 != servers.end(); it1++)
	{
		it2 = it1;
		it2++;
		for (; it2 != servers.end(); it2++)
		{
			if ((*it1).listen_port == (*it2).listen_port)
				throw(IdenticalPortNumbers());
		}
	}
}
