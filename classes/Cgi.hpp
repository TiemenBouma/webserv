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

#define CGI_READ_SIZE 100

extern	char ** environ;

enum	env_vars
{
	SERVER,
	SERVER_PROTOCOL,
	SERVER_PORT,
	REQUEST_METHOD,
	PATH_INFO,
	NULL_END
};

class	Cgi
{
	private:
		char**		_make_exec_arg(std::string program, std::string body);
	public:
		std::string	cgi(std::string program, char** env, std::string body);
		void		exiterr(std::string e);
		static char**	make_env(ConfigServer serv, Location loc, const std::string method);

	class CgiSystemFailure: public std::exception
	{
		public:
			virtual const char *	what() const throw()
			{
				return ("System call in cgi failed.");
			}
	};
};

void	cgi_get_request(Connection& connection);
void	cgi_post_request(Connection& connection);












#endif