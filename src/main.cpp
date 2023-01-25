/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: swofferh <swofferh@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/25 14:53:58 by swofferh      #+#    #+#                 */
/*   Updated: 2023/01/25 17:22:31 by swofferh      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream> // std::
#include <fcntl.h>

#include "../includes/webserver.h"

int	main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Error. Need a config file" << std::endl;
		std::exit(EXIT_FAILURE);
    }
    
	webserver();
	return (0);
}

