#ifndef WEBSERVER_H
#define WEBSERVER_H
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
#include "../src/Request.hpp"
#include "../src/Response.hpp"
#include <stdlib.h> // for exit()
#include <climits>
#include <map>


#define MIME_TYPES_FILE "../data/MIME_TYPES.txt"
#define HOMEPAGE_FILE "../data/homepage.html"
#define ROOT_DIR "../data/"
const int PORT = 8080;
const int MAX_CONNECTIONS = FD_SETSIZE - 100;
const int BUFFER_SIZE = 1024;
typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

//INITIALIZATION
int	init_server(int port, int max_connections);
void init_mime_types(std::map<std::string, std::vector<std::string> > & mime_types) ;
void init_mime_types_reverse(std::map<std::string, std::string> & mime_types_rev);

//SERVER RUNTIME
int	accept_new_connection(int server_sock);
void handle_connection(int client_socket, std::map<std::string, std::vector<std::string> > & mime_types, std::map<std::string, std::string>  mime_types_rev);
int execute_request(Request &req, Response &resp);

//GET POST DELETE
int get_request(Request &req, Response &resp);
int post_request(Request &client_request, Response &resp);




#endif