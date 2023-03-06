#include "webserver.h"
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>
#include <string.h>
#include <fcntl.h>
typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

void	init_server(std::vector<ConfigServer> &servers) {
	SA_IN server_addr;
	int option = 1;
	for (size_t i = 0; i < servers.size(); i++) {
		servers[i].server_soc = socket(AF_INET, SOCK_STREAM, 0);
		setsockopt(servers[i].server_soc, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
		if (servers[i].server_soc < 0) 
			error_message("socket() protected, returned error", 10);
		if (fcntl(servers[i].server_soc, F_SETFL, O_NONBLOCK) == -1)
			error_message("fcntl() call failed in initilisation webserver. Exit webserver", 1);

		//[INFO] set server address
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(servers[i].listen_port);
		server_addr.sin_addr.s_addr = INADDR_ANY;

		//[INFO] bind socket to address
		if (bind(servers[i].server_soc, (SA *) &server_addr, sizeof(server_addr)) < 0) 
			error_message("bind() protected, returned error", 20);

		//[INFO] listen for connections
		if (listen(servers[i].server_soc, MAX_CONNECTIONS) < 0)
			error_message("listen() protected, returned error", 30);
	}
}

void	add_server_ports(std::vector<struct pollfd> &fds, std::vector<ConfigServer> &servers) {
	for (size_t i = 0; i < servers.size(); i++) {
		fds.at(i).fd = servers[i].server_soc;
	}
}
