#include "../includes/webserver.h"

int main() {
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

int	init_server(int port, int max_connections) {
	int server_socket;
	SA_IN server_addr; 

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) {
		std::cerr << "Error: " << strerror(errno) << std::endl;
		exit(1);
	}

		// set server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	// bind socket to address
	if (bind(server_socket, (SA *) &server_addr, sizeof(server_addr)) < 0) {
		std::cerr << "Error: " << strerror(errno) << std::endl;
		exit(2);
	}

	// listen for connections
	if (listen(server_socket, max_connections) < 0) {
		std::cerr << "Error: " << strerror(errno) << std::endl;
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

void handle_connection(int client_socket) {

		char buffer[BUFFER_SIZE];
			// read request
		memset(buffer, 0, BUFFER_SIZE);
		read(client_socket, buffer, BUFFER_SIZE - 1);
		std::cout << "SERVER: Received request: " << std::endl;
		Request client_request(buffer);//PARSE REQUEST
		Response resp;
		std::cout << std::endl << client_request << std::endl;

		// Here we need a function to decide what response we do
		//now an example just with GET
		execute_request(client_request, resp);
		//get_request(client_request, resp);

		// send response
		std::string response = resp.serialize();// "HTTP/1.1 200 OK\r\nContent-Type: text/png\r\nContent-Length: 200000000000\r\n\r\n";
		//response += resp.get_body();
		write(client_socket, response.c_str(), response.size());
		close(client_socket);
}

void error_check(int succes, std::string msg) {
	if (!succes) {
		std::cout << msg << std::endl;
		exit(1) ;
	}
}

void perror_check(int succes, std::string msg) {
	if (!succes) {
		perror(msg.c_str());
		//std::cout << msg << std::endl;
		exit(1) ;
	}
}
