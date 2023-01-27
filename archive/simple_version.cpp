
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) 
{
    // read in the config file
    ifstream configFile("../data/config");
    string line;
    int portno;
    while (getline(configFile, line)) {
        if (line.find("port") == 0) {
            // extract the port number from the line
            portno = stoi(line.substr(line.find("=") + 1));
        }
    }

    // create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    // set server address and port
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // bind the socket to the address and port
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

    // listen for incoming connections
    listen(sockfd,5);
    socklen_t clilen = sizeof(struct sockaddr_in);

    // accept a connection
    int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) 
        error("ERROR on accept");

    // read from socket
    char buffer[256];
    bzero(buffer,256);
    int n = read(newsockfd, buffer, 255);
    if (n < 0) 
        error("ERROR reading from socket");
    cout << "Here is the message: " << buffer << endl;

    // write to socket
    n = write(newsockfd,"I got your message",18);
    if (n < 0) 
        error("ERROR writing to socket");

    close(newsockfd);
    close(sockfd);
    return 0; 
}
