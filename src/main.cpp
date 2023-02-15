<<<<<<< HEAD
/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: swofferh <swofferh@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/25 14:53:58 by swofferh      #+#    #+#                 */
/*   Updated: 2023/02/08 12:07:17 by svos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream> // std::
#include <fstream> 
#include <string>
#include <fcntl.h>
#include "config.hpp"

#include "../includes/webserver.h"
=======
#include <fstream> 
#include "webserver.h"

void error_msg(const char *msg, int code)
{
	perror(msg);
    exit(code);
}
>>>>>>> tiemen

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
<<<<<<< HEAD
	// [INFO] server nneds a config file
	if (argc != 2)
		error_msg("Expected: ./webserv config_file", EXIT_FAILURE);
	// read in the config file
    while (std::getline(configFile, line)) {
		std::cout << line << std::endl;
=======
	// [INFO] server needs a config file
	if (argc != 2)
		error_msg("Expected: ./webserv config_file", EXIT_FAILURE);
    while (std::getline(configFile, line)) {
	//	std::cout << line << std::endl;
>>>>>>> tiemen
		fullInput += line;
		fullInput += '\n';
    }
	try
	{
		parse_config(fullInput, servers);
<<<<<<< HEAD
		print_servers(servers);
=======
		//print_servers(servers);
>>>>>>> tiemen
	}
	catch (std::exception& e)
	{
		std::cout << "exception caught: " << std::endl;
		std::cout << e.what() << std::endl;
	}
<<<<<<< HEAD
	// start_webserver(servers.listen_port);
	return (0);
}

void error_msg(const char *msg, int code)
{
	perror(msg);
    exit(code);
}
=======
	//servers[0].print_locations(servers[0].locations);
	start_webserver(servers);
	return (0);
}


>>>>>>> tiemen

