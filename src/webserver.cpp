#include "../includes/webserver.h"
#include "classes/Config.hpp"
#include "classes/Connection.hpp"
#include <vector>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <sys/poll.h>
#include <sys/time.h>
#include <sys/types.h>





int	accept_new_connection(int server_sock) {
	//std::cout << "[DEBUG]: accept_new_connection" << std::endl;
	int addr_len = sizeof(SA_IN);
	int client_socket;
	SA_IN client_addr;
	client_socket = accept(server_sock, (SA *) &client_addr, (socklen_t *) &addr_len);
	if (!client_socket)
		exit(1);
	return client_socket;
}

int main() {
	std::vector<ConfigServer>							servers;
	std::vector<Connection>								connections;
	// int 												server_socket, 
	// 													client_socket;
	map_str_vec_str										mime_types;
	map_str_str											mime_types_rev;
	init_mime_types(mime_types);
	init_mime_types_reverse(mime_types_rev);

	//TEST SERVER
	ConfigServer test_server;
	test_server.server_name = "localhost";
	test_server.listen_port = 8080;
	test_server.root = ROOT_DIR;
	servers.push_back(test_server);
	size_t 												total_ports = servers.size();
	//std::cout << "[DEBUG]total ports: " << total_ports << std::endl;

	init_server(servers);

	// [INFO]init pollfd
	struct pollfd init_fds = {-1, POLLIN, 0};
	std::vector<struct pollfd> fds(servers.size(), init_fds);
	add_server_ports(fds, servers);
	//std::cout << "[DEBUG]fds fd: " << fds[0].fd << std::endl;

	// [INFO]handle connections
	while (true) {
		//std::cout << "[DEBUG]polling" << std::endl;
		if (poll(&*fds.begin(), fds.size(), 0) < 0) {
			std::cerr <<"Error: Poll: Exit webserver.\n";
			exit(1);
		}
		//std::cout << "[DEBUG]polling1" << std::endl;

		for (size_t i = 0; i < total_ports; i++) {
			if (!(fds[i].revents & POLLIN)) {
				continue;
			}
			//std::cout << "[DEBUG]server: " << servers[i].server_name << " port: " << servers[i].listen_port << std::endl;
			Connection new_connection(servers[i], mime_types, mime_types_rev);
			new_connection._socket = accept_new_connection(servers[i].server_soc);
			fcntl(new_connection._socket, F_SETFL, O_NONBLOCK);
			connections.push_back(new_connection);
		}
		//std::cout << "[DEBUG]polling2" << std::endl;

		int total_connections = connections.size();
		// if (total_connections > 0)
		// 	std::cout << "[DEBUG]total connections: " << total_connections << std::endl;
		for (int i = 0; i < total_connections; i++) {
			// std::cout << "[DEBUG]for loop receive/handle: " << i << std::endl;
			if (!(fds[i].revents & POLLIN)) {
				// std::cout << "[DEBUG]no pollin" << std::endl;
				continue;
			}
			receive_request(connections[i]);
			//[INFO]this is a new connection that we can accept
			//servers[i].client_socs.push_back(accept_new_connection(servers[i].server_soc));
			//fds[i].fd = servers[i].client_socs.back();
			//fds[i].events = POLLIN;
			execute_request(connections[i]);
			fds[i].fd = -1;
		}
		//std::cout << "[DEBUG]polling3" << std::endl;

	}
	return 0;
}
