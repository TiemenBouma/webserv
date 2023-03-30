
#include <iostream> // using std::cerr, std::endl, std::string
#include <unistd.h> // using close()
#include <sys/socket.h> // using socket(), bind(), listen(), accept(), recv(), send()
#include <arpa/inet.h> // using sockaddr_in
#include <string> // using std::string

std::string generate_autoindex_html(const std::string& root_dir) 
{
    std::string html = "<html>\n";
    html += "<head><title>Index of " + root_dir + "</title></head>\n";
    html += "<body>\n";
    html += "<h1>Index of " + root_dir + "</h1>\n";
    html += "<hr>\n";
    html += "<pre>\n";

    DIR* dir;
    struct dirent* entry;
    if ((dir = opendir(root_dir.c_str())) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_DIR) {
                html += "<a href=\"" + entry->d_name + "/\">" + entry->d_name + "/</a>\n";
            }
        }
        closedir(dir);
    } else {
        html += "Error: could not open directory\n";
    }

    html += "</pre>\n";
    html += "<hr>\n";
    html += "</body>\n";
    html += "</html>\n";

    return html;
}

int main() {
    int server_socket, client_socket;
    sockaddr_in server_address, client_address;
    socklen_t client_len;
    char buf[1024];

    // create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // bind socket to address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY; // <arpa/inet.h>
    server_address.sin_port = htons(8080); // <arpa/inet.h>
    if (bind(server_socket, (sockaddr*) &server_address, sizeof(server_address)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    // listen for connections
    listen(server_socket, 5);

    // accept incoming connections
    while (true) {
        client_len = sizeof(client_address);
        client_socket = accept(server_socket, (sockaddr*) &client_address, &client_len);
        if (client_socket < 0) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        // read incoming data
        int bytes_received = recv(client_socket, buf, sizeof(buf), 0);
        if (bytes_received < 0) {
            std::cerr << "Error reading from socket" << std::endl;
            close(client_socket);
            continue;
        }

        // check for autoindex request
        std::string request(buf, bytes_received);
        bool is_autoindex_request = request.find("GET /autoindex HTTP/1.1") != std::string::npos;

        // send response
        if (is_autoindex_request) {
            std::string response = generate_autoindex_html(".");
            send(client_socket, response.c_str(), response.length(), 0);
        } else {
            std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>Hello, World!</h1>";
            send(client_socket, response.c_str(), response.length(), 0);
        }

        // close connection
        close(client_socket);
    }

    // close socket
    close(server_socket);

    return 0;
}
