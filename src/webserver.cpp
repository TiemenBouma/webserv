#include "../includes/webserver.h"
#include "Config.hpp"
#include "Connection.hpp"
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

void	receive_request(Connection &connection) {
	char								buffer[1024] = {0};
	long								read_ret;
	std::string							value;
	struct pollfd 						poll_fd;

	poll_fd.events = POLLIN | POLLOUT;
	poll_fd.revents = 0;
	poll_fd.fd = connection._socket;
	read_ret = 1;

	poll(&poll_fd, 1, 0);
	if (poll_fd.revents & POLLHUP) {
		//connection ended can close up
		//code
		return ;
	}
	if (connection._request.state == RS_START) {
		if (poll_fd.revents & POLLIN) {
			read_ret = read(connection._socket, buffer, 1024 * 8);
			if (read_ret < 0) {
				connection._request.state = RS_CANCELLED;
				return;
			}
			if (!read_ret) {
				connection._request.state = RS_CANCELLED;
				return;
			}
			connection._request.state = RS_READ_ONCE;
			connection._request.whole_request += std::string(buffer, buffer + read_ret);
		}
		return;
	}
	if (connection._request.state == RS_READ_ONCE) {
		if (poll_fd.revents & POLLIN) {
			read_ret = read(socket, buffer, BUFSIZE);
			if (read_ret < 0) {
				//maybe this should just remove the connection?
				//we are not allowed to check errno
				connection._request.state = RS_CANCELLED;
				return;
			}
			if (!read_ret) {
				connection._request.state = RS_CANCELLED;
				return;
			}

			connection._request.state = RS_READ_ONCE;
			connection._request.whole_request += std::string(buffer, buffer + read_ret);
		}
	}

}

int main() {
	std::vector<ConfigServer>							servers;
	std::vector<Connection>								connections;
	int 												server_socket, 
														client_socket;
	std::map<std::string, std::vector<std::string > >	mime_types;
	std::map<std::string, std::string> 					mime_types_rev;
	int 												total_ports = servers.size();
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
			std::cerr <<"Error: Poll: Exit webserver.\n";
			exit(1);
		}
		for (size_t i = 0; i < total_ports; i++) {
			Connection new_connection(servers[i]);
			new_connection.fd = accept_new_connection(servers[i].server_soc);
			fcntl(new_connection.fd, F_SETFL, O_NONBLOCK);
			connections.push_back(new_connection);
		}
		int size_conn = connections.size();
		for (int i = 0; i < size_conn; i++) {
			if (!fds[i].revents & POLLIN)
				continue;
			receive_request(connections[i]);
			//[INFO]this is a new connection that we can accept
			//servers[i].client_socs.push_back(accept_new_connection(servers[i].server_soc));
			//fds[i].fd = servers[i].client_socs.back();
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
