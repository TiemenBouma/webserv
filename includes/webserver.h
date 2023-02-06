#ifndef WEBSERVER_H
#define WEBSERVER_H
#include "../src/classes/Config.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <csignal>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../src/classes/Request.hpp"
#include "../src/classes/Response.hpp"
#include "../src/classes/Connection.hpp"

#include <stdlib.h> // for exit()
#include <climits>
#include <map>
#include <fcntl.h>



#define MIME_TYPES_FILE "../data/MIME_TYPES.txt"
#define HOMEPAGE_FILE "../data/homepage.html"
#define ROOT_DIR "../data/"
const int PORT = 8080;
const int MAX_CONNECTIONS = FD_SETSIZE - 100;
const int BUFFER_SIZE = 1024;
typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

//INITIALIZATION
void	init_server(std::vector<ConfigServer> &servers);
void	add_server_ports(std::vector<struct pollfd> &fds, std::vector<ConfigServer> &servers);

void init_mime_types(std::map<std::string, std::vector<std::string> > & mime_types) ;
void init_mime_types_reverse(std::map<std::string, std::string> & mime_types_rev);

//SERVER RUNTIME
int	accept_new_connection(int server_sock);
void	receive_request(Connection &connection);
void handle_connection(Connection &connection);
int execute_request(Connection &connection);

//GET POST DELETE
int get_request(Connection &connection);
int post_request(Connection &connection);




#endif