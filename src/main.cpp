#include <iostream> // std::
#include <fstream> 
#include <string>
#include <fcntl.h>
#include "config.hpp"

#include "../includes/webserver.h"

int	main(int argc, char *argv[])
{
	std::ifstream configFile(argv[1]);
	std::string line;
	std::string	fullInput;
	std::vector<ConfigServer> servers;

	// this checks the state of the configFile ifstream.
	// std::cout << configFile.good() << std::endl;
	// std::cout << configFile.eof() << std::endl;
	// std::cout << configFile.fail() << std::endl;
	// std::cout << configFile.bad() << std::endl;
	// [INFO] server needs a config file
	if (argc != 2)
		error_msg("Expected: ./webserv config_file", EXIT_FAILURE);
    while (std::getline(configFile, line)) {
		std::cout << line << std::endl;
		fullInput += line;
		fullInput += '\n';
    }
	try
	{
		parse_config(fullInput, servers);
		print_servers(servers);
	}
	catch (std::exception& e)
	{
		std::cout << "exception caught: " << std::endl;
		std::cout << e.what() << std::endl;
	}
	// start_webserver(servers.listen_port);
	return (0);
}

void error_msg(const char *msg, int code)
{
	perror(msg);
    exit(code);
}

