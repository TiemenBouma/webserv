/**
 * @file main.cpp
 * @author tbouma swofferh svos
 * 
 * @brief Webserv 42 network 
 * @version 0.1
 *	
 * @date 2023-03-29
 * @copyright Copyright (c) 2023
 */

#include <fstream> 
#include <webserver.h>
#include <colors.h>

void error_message(string msg, int code) 
{
	cerr << MAGENTA << endl;
	if (!code)
		cerr << "Error: Exception caught" << endl;
	else
		perror("Error");
	cerr << BLUE << msg << endl; 
	cerr << RESET << endl;
	exit(code);
}

int	main(int argc, char *argv[])
{
	vector<ConfigServer> servers;
	ifstream configFile(argv[1]);
	string	fullInput;
	string 	line;

	// [INFO] server needs a config file.
	if (argc != 2)
		error_message("Expected input: [./webserv] [config file]", EXIT_FAILURE);

	// [INFO] checking for valid configFile.
	if (configFile.good() == false)
		error_message("Invalid config file. Try [data/config_tiemen.conf]", EXIT_FAILURE);

	// [INFO] reading file and spliting lines.
    while (getline(configFile, line)) {
		fullInput += line;
		fullInput += '\n';
    }
	// [INFO] Parsing config file, checking errors.
	try {
		parse_config(fullInput, servers);
	}
	catch (exception& e) {
		error_message(e.what(), 0);
	}
	// [INFO] printing elements of parsed servers/locations.
	cout << CYAN << endl;
	print_servers(servers);
	cout << RESET << endl;
	// [DEBUG] you can print the locations inside print_servers.

	start_webserver(servers);
	return (0);
}
