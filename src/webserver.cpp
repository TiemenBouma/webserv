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




int	accept_new_connection(int server_sock) {
	int addr_len = sizeof(SA_IN);
	int client_socket;
	SA_IN client_addr;
	client_socket = accept(server_sock, (SA *) &client_addr, (socklen_t *) &addr_len);
	if (!client_socket)
		exit(1);
	return client_socket;
}

// //[CHECK]read request from client might needs improvement to first read the header and then see how many bytes to read the body.
// int read_request(int client_socket, std::stringstream & request_data) {
// 	uint8_t buffer[BUFFER_SIZE];
// 	while (1) {
		
// 		int bytes_read = read(client_socket, buffer, BUFFER_SIZE);
// 		if (bytes_read < 0) {
// 			std::cerr << "Error read: " << strerror(errno) << std::endl;
// 			return (1);
// 		}
// 		if (bytes_read == 0 || bytes_read < BUFFER_SIZE) {
// 			if (bytes_read > 0)
// 				request_data << buffer;
// 			return 0;
// 		}
// 		request_data << buffer;
// 	}
// }



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

	// [INFO]init pollfd
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
			new_connection._socket = accept_new_connection(servers[i].server_soc);
			fcntl(new_connection._socket, F_SETFL, O_NONBLOCK);
			connections.push_back(new_connection);
		}
		int total_connections = connections.size();
		for (int i = 0; i < total_connections; i++) {
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
