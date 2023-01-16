#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <csignal>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

const int PORT = 8080;
const int MAX_CONNECTIONS = 5;
const int BUFFER_SIZE = 1024;

int main(int argc, char* argv[]) {
    int sockfd, new_sockfd;
    struct sockaddr_in server_addr, client_addr;
    int client_len;
    char buffer[BUFFER_SIZE];

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return 1;
    }

    // set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // bind socket to address
    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return 2;
    }

    // listen for connections
    if (listen(sockfd, MAX_CONNECTIONS) < 0) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return 3;
    }

    // handle connections
    while (true) {
        client_len = sizeof(client_addr);
        new_sockfd = accept(sockfd, (struct sockaddr *) &client_addr, (socklen_t *) &client_len);
        if (new_sockfd < 0) {
            std::cerr << "Error: " << strerror(errno) << std::endl;
            return 4;
        }

        // read request
        memset(buffer, 0, BUFFER_SIZE);
        read(new_sockfd, buffer, BUFFER_SIZE - 1);
        std::cout << "Received request: " << buffer << std::endl;
		write(1, buffer, strlen(buffer));
		std::cout << std::endl;

        // send response
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello, World!";
        write(new_sockfd, response.c_str(), response.size());
        close(new_sockfd);
    }

    return 0;
}