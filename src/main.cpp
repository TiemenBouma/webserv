/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: swofferh <swofferh@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/25 14:53:58 by swofferh      #+#    #+#                 */
/*   Updated: 2023/02/01 14:27:02 by svos          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

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
	ConfigServer configData;

	// this checks the state of the configFile ifstream.
	// std::cout << configFile.good() << std::endl;
	// std::cout << configFile.eof() << std::endl;
	// std::cout << configFile.fail() << std::endl;
	// std::cout << configFile.bad() << std::endl;
	// [INFO] server nneds a config file
	if (argc != 2)
		error_msg("Expected: ./webserv config_file", EXIT_FAILURE);
	// read in the config file
    while (std::getline(configFile, line)) {
		std::cout << line << std::endl;
        if (line.find("	listen") == 0) {
            configData.listen_port = stoi(line.substr(line.find(" ") + 1));
        }
		fullInput += line;
    }
	std::cout << "listen port: " << configData.listen_port << std::endl;
	if (configData.check_brackets(fullInput) == 0)
		error_msg("incorrect brackets", EXIT_FAILURE);
	try
	{
		configData.parse_config(fullInput, configData);
	}
	catch (std::exception& e)
	{
		std::cout << "exception caught: " << std::endl;
		std::cout << e.what() << std::endl;
	}
	start_webserver(configData.listen_port);
	return (0);
}

void error_msg(const char *msg, int code)
{
	perror(msg);
    exit(code);
}

