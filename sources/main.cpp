#include <fstream> 
#include "webserver.h"
#include "../classes/Config.hpp"

static const std::string COLOUR = "\x1b[34m";
static const std::string RESET = "\x1b[0m\n\n";

void error_msg(std::string msg, int code) {
	perror("\nError");
	std::cout << COLOUR << msg << RESET; 
	exit(code);
}

int	main(int argc, char *argv[])
{
	std::vector<ConfigServer> servers;
	std::ifstream configFile(argv[1]);
	std::string	fullInput;
	std::string line;

	// [INFO] server needs a config file.
	if (argc != 2)
		error_msg("Expected input: [./webserv] [config file]", EXIT_FAILURE);

	// [INFO] checking for valid configFile.
	if (configFile.good() == false)
		error_msg("Invalid config file. Try [data/config_tiemen.conf]", EXIT_FAILURE);

	// [INFO] reading file and 
    while (getline(configFile, line)) {
		fullInput += line;
		fullInput += '\n';
    }
	try {
		parse_config(fullInput, servers);
		std::cout << "DEBUG: configuration file content: " << std::endl;
		print_servers(servers);
	}
	catch (std::exception& e) {
		std::cout << "exception caught: " << std::endl;
		std::cout << e.what() << std::endl;
		exit(1);
	}
	//servers[0].print_locations(servers[0].locations);
	start_webserver(servers);
	return (0);
}
