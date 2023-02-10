#include "webserver.h"
#include "typedef.h"
#include <sys/poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h> //for close ()


int	accept_new_connection(int server_sock) {
	int addr_len = sizeof(SA_IN);
	int client_socket;
	SA_IN client_addr;
	client_socket = accept(server_sock, (SA *) &client_addr, (socklen_t *) &addr_len);
	if (!client_socket)
		exit(1);
	return client_socket;
}

int start_webserver(std::vector<ConfigServer> servers) {
	std::vector<Connection>								connections;
	map_str_vec_str										mime_types;
	map_str_str											mime_types_rev;
	init_mime_types(mime_types);
	init_mime_types_reverse(mime_types_rev);

	//TEST SERVER
	// ConfigServer test_server;
	// test_server.server_name = "localhost";
	// test_server.listen_port = 8080;
	// test_server.root = ROOT_DIR;
	// servers.push_back(test_server);
	size_t												total_ports = servers.size();

	init_server(servers);

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

		for (size_t i = 0; i < total_ports; i++) {
			if (!(fds[i].revents & POLLIN)) {
				continue;
			}
			Connection new_connection(servers[i], mime_types, mime_types_rev);
			new_connection._socket = accept_new_connection(servers[i].server_soc);
			//[INFO] new connection socket is not active with POLLIN :(
			struct pollfd new_pollfd = {new_connection._socket, POLLIN, 0};
			poll(&new_pollfd, 1, 0);
			if (new_pollfd.revents & POLLIN) {
				cout << "[DEBUG]POLLIN new connection Active\n";
			}
			if (new_connection._socket < 0)
				continue;
			fcntl(new_connection._socket, F_SETFL, O_NONBLOCK);
			connections.push_back(new_connection);
		}

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
				execute_request(connections[i]);
			if (connections[i]._request._state == REQUEST_DONE ||
				connections[i]._request._state == REQUEST_CANCELLED) {
				close(connections[i]._socket);
				connections.erase(connections.begin() + i);
			}
		}
	}
	return 0;
}
