/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: swofferh <swofferh@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/25 14:53:58 by swofferh      #+#    #+#                 */
/*   Updated: 2023/01/27 17:38:36 by swofferh      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream> // std::
#include <fstream>	// ifstream
#include <string>	// stoi
#include <fcntl.h> 
#include "config.hpp"
#include "../includes/webserver.h"

int	main(int argc, char *argv[])
{
	std::ifstream configFile(argv[1]);
	std::string line;
	ConfigFile configData;

	// [INFO] this checks the state of the configFile ifstream.
	// std::cout << configFile.good() << std::endl;
	// std::cout << configFile.eof() << std::endl;
	// std::cout << configFile.fail() << std::endl;
	// std::cout << configFile.bad() << std::endl;

	// [INFO] server nneds a config file
	if (argc != 2)
	{
		std::cerr << "Error. Need a config file" << std::endl;
		std::exit(EXIT_FAILURE);
    }
	// read in the config file
    while (std::getline(configFile, line)) {
        if (line.find("	listen") == 0) {
            configData.port = stoi(line.substr(line.find(" ") + 1));
        }
		if (line.find("	root") == 0) {
            configData.root = line.substr(line.find(" ") + 1);
		}
		if (line.find("	location") == 0) {
            configData.root = line.substr(line.find(" ") + 1);
		}
	}
	start_webserver(configData);
	return (0);
}

int error_smg(const char *msg, int code)
{
	perror(msg);
    exit(code);
}

