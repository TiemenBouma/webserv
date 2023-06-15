#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h> // for exit()

#include "Connection.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Config.hpp"
#include "typedef.h"
#include "colors.h"

using namespace std;

//INITIALIZATION
int     start_webserver(std::vector<ConfigServer> servers);
void	init_server(std::vector<ConfigServer> &servers);
void	add_server_ports(std::vector<struct pollfd> &fds, std::vector<ConfigServer> &servers);

void    init_mime_types(std::map<std::string, std::vector<std::string> > & mime_types) ;
void    init_mime_types_reverse(std::map<std::string, std::string> & mime_types_rev);
int		parse_config(std::string config, std::vector<ConfigServer> &servers);

//SERVER RUNTIME
int	    accept_new_connection(int server_sock);
void	receive_request(Connection &connection);
void    handle_connection(Connection &connection);
int		init_response(Connection &connection);
int     execute_request(Connection &connection);

//GET POST DELETE
int     get_request(Connection &connection);
int     post_request(Connection &connection);
int     delete_request(Connection &connection);

//ERROR HANDLING
int     error_request(Connection &connection);
void    error_message(std::string msg, int code);

//PARSERSING/CONFIG
int	    parse_config(std::string config, std::vector<ConfigServer> &servers);
void	print_servers(std::vector<ConfigServer> servers);
int	    check_brackets(std::string config);
int	    skipspace(std::string::iterator it);
string	it_to_str(std::string::iterator it);
bool	check_conf_ext(std::string	filename);

//AUTOINDEX
int		autoindex_get(Connection &connection, string &dir);



#endif