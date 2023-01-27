/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: swofferh <swofferh@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/25 14:53:58 by swofferh      #+#    #+#                 */
/*   Updated: 2023/01/27 15:23:03 by swofferh      ########   odam.nl         */
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
	std::ifstream configFile(argv[2]);
	std::string line;
	ConfigServer configData;

	// [INFO] server nneds a config file
	if (argc != 2)
	{
		std::cerr << "Error. Need a config file" << std::endl;
		std::exit(EXIT_FAILURE);
    }
	// read in the config file
    while (getline(configFile, line)) {
        if (line.find("	listen") == 0) {
            configData.listen_port = line.substr(line.find(" "));
        }
    }
		std::cout << line << std::endl;
	start_webserver(8080);
	return (0);
}

int error_smg(const char *msg, int code)
{
	perror(msg);
    exit(code);
}

