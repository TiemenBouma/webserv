#include "config.hpp"
#include <iostream>
#include <stack>

int	ConfigServer::check_brackets(std::string config)
{
	std::stack<char>	stack;
	int					brack_state = 0;

	//	push all curly brackets to a stack
	for (std::string::iterator it = config.begin(); it != config.end(); it++)
	{
		if (*it == '{' || *it == '}')
			stack.push(*it);
	}
	std::cout << "stack size: " << stack.size() << std::endl;
	//	check if the amount of left brackets are equal to the amount of right ones
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

// int	ConfigServer::parse_config(std::string config)
// {
// 	//look for the string 'server', then go in server parser. parse server stuf. if string location is found, go in location parser.
// }

//	Checks if curly brackets in config file are placed correctly.
//	returns 1 for success, 0 or -1 when failed.
// int	ConfigServer::check_brackets(std::string config, std::string::iterator it)
// {
// 	std::cout << "entered function" << std::endl;
// 	for (; it != config.end(); it++)
// 	{
// 		std::cout << "iterating over: " << *it << std::endl;
// 		if (*it == '{')
// 		{
// 			if (ConfigServer::check_brackets(config, it + 1) != 0)
// 				return (-1);
// 		}
// 		if (*it == '}')
// 			return (0);
// 	}
// 	std::cout << "entered check_brackets function" << std::endl;
// 	(void) config;
// 	return (1);
// }

// std::vector<std::string> tokenize(std::string conf) {
// 	std::string::size_type 		end;
// 	std::vector<std::string>	tokens;

// 	for(std::string::size_type i = 0; i < conf.length();)
// 	{
// 		/*special characters*/
// 		if (conf[i] == '{' || conf[i] == ';' || conf[i] == '}') {
// 			std::string token(&conf[i], 1);
// 			tokens.push_back(token);
// 			i++;
// 			continue;
// 		/*whitespace*/
// 		} else if (conf[i] == ' ' || conf[i] == '\n' || conf[i] == '\t') {
// 			i++;
// 			continue;
// 		/*comments*/
// 		} else if (conf[i] == '#') {
// 			end = conf.find_first_of("\n", i);
// 			i = end;
// 		/*other strings*/
// 		} else {
// 			end = conf.find_first_of(" \t\n;{}", i);
// 			if(end == std::string::npos)
// 			{
// 				end = conf.length();
// 			}
// 			std::string token(&conf[i], end-i);
// 			tokens.push_back(token);
// 			i = end;
// 		}
// 	}
//    return tokens;
// }