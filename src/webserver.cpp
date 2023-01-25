#include "../includes/webserver.h"
#include <vector>
#include <sstream>

int	init_server(int port, int max_connections) {
	int server_socket;
	SA_IN server_addr; 

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) {
		std::cerr << "Error socket: " << strerror(errno) << std::endl;
		exit(1);
	}

		// set server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	// bind socket to address
	if (bind(server_socket, (SA *) &server_addr, sizeof(server_addr)) < 0) {
		std::cerr << "Error bind: " << strerror(errno) << std::endl;
		exit(2);
	}

	// listen for connections
	if (listen(server_socket, max_connections) < 0) {
		std::cerr << "Error listen: " << strerror(errno) << std::endl;
		exit(3);
	}
	return server_socket;
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

int read_request(int client_socket, std::stringstream & request_data) {
	uint8_t buffer[BUFFER_SIZE];
	while (1) {
		std::cout << "DEBUG: Reading request" << std::endl;
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

void handle_connection(int client_socket) {
	std::cout << "DEBUG: Handling connection" << std::endl;
	// READ REQUEST
	std::stringstream request_data;
	read_request(client_socket, request_data);
	//DEBUG
	// std::cout << "BEDUG" << std::endl;
	// std::string request = request_data.str();
	// std::cout << "DEBUG: Request: " << request << std::endl;

	//PARSE REQUEST in Request class constructor
	Request client_request(request_data);
	Response resp;
	resp.set_client_socket(client_socket);

	// Here we need a function to decide what response we do
	//now an example just with GET
	execute_request(client_request, resp);
	close(client_socket);
}

int webserver() {
	int server_socket, client_socket;

	server_socket = init_server(PORT, MAX_CONNECTIONS);

	fd_set current_sockets, ready_sockets;
	FD_ZERO(&current_sockets);
	FD_SET(server_socket, &current_sockets);

	// handle connections
	while (true) {
		ready_sockets = current_sockets;
		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0) {
			perror("ERROR\n");
			exit(1);
		}
		for (int i = 0; i < FD_SETSIZE; i++) {
			if (FD_ISSET(i, &ready_sockets)) {
				if (i == server_socket) {
					//this is a new connection that we can accept
					client_socket = accept_new_connection(server_socket);
					FD_SET(client_socket, &current_sockets);
				} else {
					handle_connection(i);
					FD_CLR(i, &current_sockets);
				}
			}
		}
	}
	return 0;
}