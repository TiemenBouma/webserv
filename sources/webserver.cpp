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
	return client_socket;
}

int start_webserver(std::vector<ConfigServer> servers) {
	std::vector<Connection>								connections;
	map_str_vec_str										mime_types;
	map_str_str											mime_types_rev;
	size_t												total_ports = servers.size();

	// [INFO]init mime types, for the surfix
	init_mime_types(mime_types);
	init_mime_types_reverse(mime_types_rev);

	init_server(servers);

	// [INFO]init pollfd
	struct pollfd init_fds = {-1, POLLIN, 0};
	std::vector<struct pollfd> poll_fds(servers.size(), init_fds);
	std::vector<struct pollfd> fds_client;
	add_server_ports(poll_fds, servers);

	// [INFO]handle connections
	while (true) {
		int check = poll(&*poll_fds.begin(), poll_fds.size(), 1) < 0;
		if (check == -1)
			error_message("(Poll) protected, returned error. Exit webserver", 6);

		//[INFO]listening and accepting to new connection comming in
		for (size_t i = 0; i < total_ports; i++) {
			if (!(poll_fds[i].revents & POLLIN)) {
				continue;
			}
			Connection new_connection(servers[i], mime_types, mime_types_rev);
			new_connection._socket = accept_new_connection(servers[i].server_soc);
			
			//[INFO]if accepts() call fail Connection will not be added to connection list.
			if (new_connection._socket < 0) { 
				cout << "[SERVER] accept() call failed" << endl;
				continue;
			}

			//[INFO]if fcntl() call fail Connection will not be added to connection list.
			if (fcntl(new_connection._socket, F_SETFL, O_NONBLOCK) == -1) {
				cout << "[SERVER] fcntl() call failed" << endl;
				continue;
			}
			
			cout << endl << "[SERVER] new connection socket: " << new_connection._socket << endl;
			
			// Add the new connection to the poll_fds vector
            struct pollfd new_client_fd = {new_connection._socket, POLLIN | POLLOUT, 0};
            poll_fds.push_back(new_client_fd);
			
			connections.push_back(new_connection);
		}
		//[INFO] Handeling current connections
		size_t total_connections = connections.size();
		//cout << "DEBUG: total connections: " << total_connections << endl;
		//cout << "DEBUG: poll_fds: " << poll_fds.size() << endl;
		for (size_t con_index = 0; con_index < total_connections; con_index++) {
			size_t relative_con_index = con_index + total_ports;
			//TIMEOUT CHECK
			if (connections[con_index].check_time_out()) {
				cout << "[SERVER] TIMEOUT ACTIVATED: " << connections[con_index]._request._url << endl;
				close(connections[con_index]._socket);
				connections.erase(connections.begin() + con_index);
				poll_fds.erase(poll_fds.begin() + relative_con_index);
				total_connections = connections.size();
				con_index--;
				continue;
			}
			//cout << "DEBUG: connections: " << total_connections <<  endl;
			//cout << "DEBUG: connection state: " << connections[i]._request._state <<  endl;

			if (!(poll_fds[relative_con_index].revents & POLLOUT) && !(poll_fds[relative_con_index].revents & POLLIN)) {
				cout << "SERVER: POLL not ready" << endl;
				continue;
			}

			//cout << "[SERVER] receiving request on fd: " << connections[con_index]._socket << endl;
			if ((poll_fds[relative_con_index].revents & POLLIN))
				receive_request(connections[con_index]);
			if (connections[con_index]._request._state == REQUEST_DONE && (poll_fds[relative_con_index].revents & POLLOUT)) {
				if (connections[con_index]._response._total_send_body == 0)
					cout << "[SERVER] execute request: " << connections[con_index]._request.get_method() << " " << connections[con_index]._request.get_url() <<  endl;
				execute_request(connections[con_index]);
			}
			if (connections[con_index]._request._state == REQUEST_DONE && connections[con_index]._response._content_length == connections[con_index]._response._total_send_body) {
				connections[con_index]._request._state = REQUEST_DONE_AND_SEND;
			}
			if (connections[con_index]._request._state == REQUEST_DONE_AND_SEND ||
				connections[con_index]._request._state == REQUEST_CANCELLED) {
					cout << "[SERVER] close connection: " << connections[con_index]._request.get_method() << " " << connections[con_index]._request.get_url()  << endl;
					close(connections[con_index]._socket);
					connections.erase(connections.begin() + con_index);
					poll_fds.erase(poll_fds.begin() + relative_con_index); // Remove the corresponding entry from poll_fds vector
					con_index--;
			}
			// Update total_connections
            total_connections = connections.size();
		}
	}
	return 0;
}
