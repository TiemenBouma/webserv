#include <fstream> 
#include "webserver.h"

void error_msg(std::string msg, int code)
{
	perror(msg);
    exit(code);
}

int	main(int argc, char *argv[])
{
	std::ifstream configFile(argv[1]);
	std::string line;
	std::string	fullInput;
	std::vector<ConfigServer> servers;

	// [INFO] server needs a config file.
	if (argc != 2)
		error_msg("\n[ERROR] Expected input: [./webserv] [config file].", EXIT_FAILURE);

	// [INFO] checking for valid configFile.
	if (configFile.good() == false)
		error_msg("[ERROR] Invalid config file.", EXIT_FAILURE);


    while (std::getline(configFile, line)) {
	//	std::cout << line << std::endl;
		fullInput += line;
		fullInput += '\n';
    }
	try
	{
		parse_config(fullInput, servers);
		std::cout << "DEBUG: configuration file content: " << std::endl;
		print_servers(servers);
	}
	catch (std::exception& e)
	{
		std::cout << "exception caught: " << std::endl;
		std::cout << e.what() << std::endl;
	}
	//servers[0].print_locations(servers[0].locations);
	start_webserver(servers);
	return (0);
}
