#ifndef CGI_HPP
#define CGI_HPP
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/poll.h>
#include "Config.hpp"
#include "Connection.hpp"

#define PATH_INFO ""
#define CGI_READ_SIZE 100

extern	char ** environ;

class	Cgi
{
	private:
		char**		_make_exec_arg(std::string program, std::string path_info);
	public:
		std::string	cgi(std::string program, std::string path_info);
		void		exiterr(std::string e);

	class CgiSystemFailure: public std::exception
	{
		public:
			virtual const char *	what() const throw()
			{
				return ("System call in cgi failed.");
			}
	};
};
bool		is_cgiable(std::string location, ConfigServer server);

void	cgi_get_request(Connection& connection);












#endif