#include "../includes/webserver.h"
#include <poll.h>

void	init_server(std::vector<ConfigServer> &servers) {
	SA_IN server_addr;
	int option = 1;
	for (size_t i = 0; i < servers.size(); i++) {
		servers[i].server_soc = socket(AF_INET, SOCK_STREAM, 0);
		setsockopt(servers[i].server_soc, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
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
		//std::cout << "[DEBUG] server socket: " << servers[i].server_soc << " is listening." << std::endl;
	}
}

void	add_server_ports(std::vector<struct pollfd> &fds, std::vector<ConfigServer> &servers) {
	for (size_t i = 0; i < servers.size(); i++) {
		//std::cout << "[DEBUG] add server port: " << servers[i].listen_port << std::endl;
		fds.at(i).fd = servers[i].server_soc;
	}
}