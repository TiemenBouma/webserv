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
#include "../server/Request.hpp"
#include "../server/Response.hpp"
#include <stdlib.h> // for exit()
#include <climits>


#define MIME_TYPES_FILE "../data/MIME_TYPES.txt"
#define HOMEPAGE_FILE "../data/homepage.html"
const int PORT = 8080;
const int MAX_CONNECTIONS = FD_SETSIZE - 100;
const int BUFFER_SIZE = 1024;
typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

int	init_server(int port, int max_connections);
int	accept_new_connection(int server_sock);
void handle_connection(int client_socket);
int get_request(Request &req, Response &resp);
int execute_request(Request &req, Response &resp);

#endif