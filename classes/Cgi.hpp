#ifndef CGI_HPP
#define CGI_HPP
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>

#define PATH_INFO "utils/cgi-bin/"
#define CGI_READ_SIZE 100

extern	char ** environ;

class	Cgi
{
	private:
		char**		_make_exec_arg(std::string program, std::string path_info);
	public:
		std::string	cgi(std::string program, std::string path_info);
		void		exiterr(std::string e);

};













#endif