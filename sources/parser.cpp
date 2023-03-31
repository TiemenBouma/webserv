/**
 * @brief Created by Swenne
 * 
 *  referenced from Config Class
 *              created by Sietse
 */

#include <stack>
#include <sstream>
#include <cstring>

#include "Config.hpp"
#include "webserver.h"
#include "colors.h"

int	parse_config(std::string config, std::vector<ConfigServer> &servers)
{
	std::string::iterator	it = config.begin();

	if (check_brackets(config) == 0)
		throw(ConfigServer::UnbalancedBrackets());
	while (true)
	{
		ConfigServer	serv;

		if (*it == '}')
			it += 1;
		it += skipspace(it);
		if (*it == '\0')
			return (1);
		if (serv.cmp_directive(it, "server") == 0)
			throw(ConfigServer::UnknownKeyword());
		it += strlen("server");
		it += skipspace(it);
		if (*it != '{')
			throw(ConfigServer::NoBracketAfterServer());
		it += 1;
		it += skipspace(it);
		serv.parse_keyword(it);
		serv.check_req_direcs();
		servers.push_back(serv);
	}
	return (0);
}

int	check_brackets(std::string config)
{
	std::stack<char>	stack;
	int					brack_state = 0;

	//	[INFO] push all curly brackets to a stack
	for (std::string::iterator it = config.begin(); it != config.end(); it++)
	{
		if (*it == '{' || *it == '}')
			stack.push(*it);
	}
	//std::cout << "stack size: " << stack.size() << std::endl;
	//	[INFO]check if the amount of left brackets are equal to the amount of right ones
	while (stack.size() > 0)
	{
		if (stack.top() == '}')
			brack_state++;
		if (stack.top() == '{')
			brack_state--;
		if (brack_state <= -1)
			return (0);
		stack.pop();
	}
	if (brack_state == 0)
		return (1);
	return (0);
}

int	skipspace(std::string::iterator it)
{
	int	ret = 0;
	while (isspace(*it) != 0)
	{
		it++;
		ret++;
	}
	return (ret);
}

std::string	it_to_str(std::string::iterator it)
{
	std::string	ret = "";

	for (; *it != ' ' && *it != '\0'; it++)
		ret += *it;
	return (ret);
}

/*
	[INFO] PRINTING FUNCTIONS
*/

void	print_servers(std::vector<ConfigServer> servers)
{
	for (std::vector<ConfigServer>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		std::cout << std::endl << "Server information:" << std::endl << std::endl;
		std::cout << "Listen: '" << (*it).listen_port << "'" << std::endl;
		std::cout << "Root: '" << (*it).root << "'" << std::endl;
		std::cout << "Server name: '" << (*it).server_name << "'" << std::endl;
		std::cout << "Error pages:" << std::endl;
		for (std::map<int, std::string>::iterator mapit = (*it).error_pages.begin(); mapit != (*it).error_pages.end(); mapit++)
			std::cout << "'" << (*mapit).first << ", " << (*mapit).second << "'" << std::endl;
		std::cout << "Size content: '" << (*it).size_content << "'" << std::endl;
        // [DEBUG] if you want to print locations check above fun on Config.cpp 
		//(*it).print_locations((*it).locations);
	}
}
