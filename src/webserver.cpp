#include "webserver.h"
#include "typedef.h"
#include <sys/poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h> //for close ()

<<<<<<< HEAD
int	init_server(int port, int max_connections) {
	int server_socket;
	SA_IN server_addr; 

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) {
		std::cerr << "Error socket: " << strerror(errno) << std::endl;
		exit(1);
	}

	//[INFO] set server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	//[INFO] bind socket to address
	if (bind(server_socket, (SA *) &server_addr, sizeof(server_addr)) < 0) {
		std::cerr << "Error bind: " << strerror(errno) << std::endl;
		exit(2);
	}

	//[INFO] listen for connections
	if (listen(server_socket, max_connections) < 0) {
		std::cerr << "Error listen: " << strerror(errno) << std::endl;
		exit(3);
	}
	return server_socket;
}
=======
>>>>>>> tiemen

int	accept_new_connection(int server_sock) {
	int addr_len = sizeof(SA_IN);
	int client_socket;
	SA_IN client_addr;
	client_socket = accept(server_sock, (SA *) &client_addr, (socklen_t *) &addr_len);
	if (!client_socket)
		exit(1);
	return client_socket;
}

<<<<<<< HEAD
//[CHECK] read request from client might needs improvement to first read the header and then see how many bytes to read the body.
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
=======
int start_webserver(std::vector<ConfigServer> servers) {
	std::vector<Connection>								connections;
	map_str_vec_str										mime_types;
	map_str_str											mime_types_rev;
	size_t												total_ports = servers.size();
	
	// [INFO]init mime types, for the surfix
	init_mime_types(mime_types);
	init_mime_types_reverse(mime_types_rev);
>>>>>>> tiemen

	init_server(servers);

<<<<<<< HEAD
	//[INFO] PARSE REQUEST in Request class constructor
	std::cout << "DEBUG: Parsing request" << std::endl;
	Request client_request(request_data, mime_types, mime_types_rev);
	std::cout << "DEBUG: Parsing request finiched" << std::endl;

	//[INFO] Construct Response
	Response server_resp(mime_types, mime_types_rev);
	server_resp.set_client_socket(client_socket);
=======
	// [INFO]init pollfd
	struct pollfd init_fds = {-1, POLLIN, 0};
	std::vector<struct pollfd> fds(servers.size(), init_fds);
	add_server_ports(fds, servers);

	// [INFO]handle connections
	while (true) {
		if (poll(&*fds.begin(), fds.size(), 0) < 0) {
			std::cerr <<"Error: Poll: Exit webserver.\n";
			exit(1);
		}
		//listening  and accepting to new connection comming in
		for (size_t i = 0; i < total_ports; i++) {
			if (!(fds[i].revents & POLLIN)) {
				continue;
			}
			Connection new_connection(servers[i], mime_types, mime_types_rev);
			new_connection._socket = accept_new_connection(servers[i].server_soc);

			
			//[DEBUG]TIEMEN LOOK AT THIS
			struct pollfd new_pollfd = {new_connection._socket, POLLIN, 0};
			poll(&new_pollfd, 1, 0);
			if (new_pollfd.revents & POLLIN) {
				cout << "[DEBUG]POLLIN new connection Active\n";
			}

			if (new_connection._socket < 0) //check if this needs to be contionue or someting else
				continue;
			fcntl(new_connection._socket, F_SETFL, O_NONBLOCK);
			connections.push_back(new_connection);
		}
>>>>>>> tiemen

		//[INFO] Handeling current connections
		int total_connections = connections.size();
		for (int i = 0; i < total_connections; i++) {
			if (!(fds[i].revents & POLLIN)) {
				continue;
			}
			receive_request(connections[i]);
			//DEBUG
			if (connections[i]._request._state == REQUEST_CANCELLED)
				cout << connections[i]._request.get_error_log() << endl;
			//DEBUG END
			if (connections[i]._request._state == REQUEST_DONE)
				execute_request(connections[i]);// Maybe rename to handeling or someting like that.
			if (connections[i]._request._state == REQUEST_DONE ||
				connections[i]._request._state == REQUEST_CANCELLED) {
				close(connections[i]._socket);
				connections.erase(connections.begin() + i);
			}
		}
	}
	return 0;
}

int start_webserver(int portno) {
	int server_socket, client_socket;
	std::map<std::string, std::vector<std::string> > mime_types;
	std::map<std::string, std::string>  mime_types_rev;
	init_mime_types(mime_types);
	init_mime_types_reverse(mime_types_rev);
	server_socket = init_server(portno, MAX_CONNECTIONS);

	fd_set current_sockets, ready_sockets;
	FD_ZERO(&current_sockets);
	FD_SET(server_socket, &current_sockets);

	// [INFO]handle connections
	//std::cout << portno << std::endl;
	while (true) {
		ready_sockets = current_sockets;
		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0) {
			//[CHECK]PERROR NOT ALLOWED IN THIS PART OF SERVER?
			perror("ERROR\n");
			exit(1);
		}
		for (int i = 0; i < FD_SETSIZE; i++) {
			if (FD_ISSET(i, &ready_sockets)) {
				if (i == server_socket) {
					//[INFO]this is a new connection that we can accept
					client_socket = accept_new_connection(server_socket);
					FD_SET(client_socket, &current_sockets);
				} else {
					handle_connection(i, mime_types, mime_types_rev);
					FD_CLR(i, &current_sockets);
				}
			}
		}
	}
	return 0;
}