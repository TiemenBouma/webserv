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
    int sockfd, sockfd2, new_sockfd;
    struct sockaddr_in server_addr, client_addr;
    int client_len;
    char buffer[BUFFER_SIZE];

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return 1;
    }
    sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd2 < 0) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return 1;
    }
	std::cout << sockfd << std::endl;
	std::cout << sockfd2 << std::endl;
}