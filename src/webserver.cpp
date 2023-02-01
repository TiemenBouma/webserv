#include "../includes/webserver.h"
#include "config.hpp"
#include <vector>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <sys/poll.h>
#include <sys/time.h>
#include <sys/types.h>

// int main() {
// 	int server_socket, client_socket;
// 	std::map<std::string, std::vector<std::string> > mime_types;
// 	std::map<std::string, std::string>  mime_types_rev;
// 	init_mime_types(mime_types);
// 	init_mime_types_reverse(mime_types_rev);
// 	server_socket = init_server(PORT, MAX_CONNECTIONS);

// 	fd_set current_sockets, ready_sockets;
// 	FD_ZERO(&current_sockets);
// 	FD_SET(server_socket, &current_sockets);

// 	// [INFO]handle connections
// 	while (true) {
// 		ready_sockets = current_sockets;
// 		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0) {
// 			//[CHECK]PERROR NOT ALLOWED IN THIS PART OF SERVER?
// 			perror("ERROR\n");
// 			exit(1);
// 		}
// 		for (int i = 0; i < FD_SETSIZE; i++) {
// 			if (FD_ISSET(i, &ready_sockets)) {
// 				if (i == server_socket) {
// 					//[INFO]this is a new connection that we can accept
// 					client_socket = accept_new_connection(server_socket);
// 					FD_SET(client_socket, &current_sockets);
// 				} else {
// 					handle_connection(i, mime_types, mime_types_rev);
// 					FD_CLR(i, &current_sockets);
// 				}
// 			}
// 		}
// 	}
// 	return 0;
// }

void	add_server_ports(std::vector<struct pollfd> fds, std::vector<ConfigServer> servers) {
	for (size_t i = 0; i < servers.size(); i++) {
		fds[i].fd = servers[i].listen_port;
	}
}

int main() {
	std::vector<ConfigServer> servers;
	int server_socket, client_socket;
	std::map<std::string, std::vector<std::string > > mime_types;
	std::map<std::string, std::string> mime_types_rev;
	init_mime_types(mime_types);
	init_mime_types_reverse(mime_types_rev);
	init_server(servers);
	nfds_t size = 3;

	struct pollfd init_fds = {-1, POLLIN, 0};
	std::vector<struct pollfd> fds(servers.size(), init_fds);
	add_server_ports(fds, servers);

	// [INFO]handle connections
	while (true) {
		if (poll(&*fds.begin(), fds.size(), -1) < 0) {
			std::cerr <<"Error: Poll\n";
			exit(1);
		}
		for (int i = 0; i < FD_SETSIZE; i++) {
			if (!fds[i].revents & POLLIN)
				continue;
			//[INFO]this is a new connection that we can accept
			servers[i].client_socs.push_back(accept_new_connection(servers[i].server_soc));
			fds[i].fd = servers[i].client_socs.back();
			//fds[i].events = POLLIN;
			handle_connection(fds[i].fd, mime_types, mime_types_rev);
			fds[i].fd = -1;
		}
	}
	return 0;
}

void	init_server(std::vector<ConfigServer> &servers) {
	SA_IN server_addr;
	for (size_t i = 0; i < servers.size(); i++) {
		servers[i].server_soc = socket(AF_INET, SOCK_STREAM, 0);
		if (servers[i].server_soc < 0) {
			std::cerr << "Error socket: " << strerror(errno) << std::endl;
			exit(1);
		}

			//[INFO] set server address
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(servers[i].listen_port);
		server_addr.sin_addr.s_addr = INADDR_ANY;

		//[INFO] bind socket to address
		if (bind(servers[i].server_soc, (SA *) &server_addr, sizeof(server_addr)) < 0) {
			std::cerr << "Error bind: " << strerror(errno) << std::endl;
			exit(2);
		}

		//[INFO] listen for connections
		if (listen(servers[i].server_soc, MAX_CONNECTIONS) < 0) {
			std::cerr << "Error listen: " << strerror(errno) << std::endl;
			exit(3);
		}
	}
}

int	accept_new_connection(int server_sock) {
	int addr_len = sizeof(SA_IN);
	int client_socket;
	SA_IN client_addr;
	client_socket = accept(server_sock, (SA *) &client_addr, (socklen_t *) &addr_len);
	if (!client_socket)
		exit(1);
	return client_socket;
}

//[CHECK]read request from client might needs improvement to first read the header and then see how many bytes to read the body.
int read_request(int client_socket, std::stringstream & request_data) {
	uint8_t buffer[BUFFER_SIZE];
	while (1) {
		
		int bytes_read = read(client_socket, buffer, BUFFER_SIZE);
		if (bytes_read < 0) {
			std::cerr << "Error read: " << strerror(errno) << std::endl;
			return (1);
		}
		if (bytes_read == 0 || bytes_read < BUFFER_SIZE) {
			if (bytes_read > 0)
				request_data << buffer;
			return 0;
		}
		request_data << buffer;
	}
}

void handle_connection(int client_socket, std::map<std::string, std::vector<std::string> > & mime_types
		, std::map<std::string, std::string>  mime_types_rev) {
	std::cout << "DEBUG: Handling connection" << std::endl;
	// [INFO] READ REQUEST
	std::stringstream request_data;
	std::cout << "DEBUG: Reading request" << std::endl;
	read_request(client_socket, request_data);
	std::cout << "DEBUG: Reading request finished" << std::endl;


	//[INFO]PARSE REQUEST in Request class constructor
	std::cout << "DEBUG: Parsing request" << std::endl;
	Request client_request(request_data, mime_types, mime_types_rev);
	std::cout << "DEBUG: Parsing request finiched" << std::endl;
	
	//[INFO]Construct Response

	Response server_resp(mime_types, mime_types_rev);
	server_resp.set_client_socket(client_socket);

	// [INFO] Here we need a function to decide what response we do
	//now an example just with GET
	std::cout << "DEBUG: Executing request" << std::endl;
	execute_request(client_request, server_resp);
	close(client_socket);
}
