#include "Response.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
// //#include <map>
#include <unistd.h>


Response::Response(map_str_vec_str &mime_types, map_str_str &mime_types_rev)
:	 
	_http_version("HTTP/1.1"), 
	_status_code("200"), 
	_status_message("OK"), 
	_headers(""), 
	_header_content_type(""), 
	_header_content_length(""),
	_content_length(0),
	_body(""),
	_total_send_body(0),
	_location_server(NULL),
	_file_path(""),
	_ifstream_response(NULL),
	_client_socket(-1),
	_mime_types(mime_types),
	_mime_types_rev(mime_types_rev) 
	{}

Response::Response(const Response &other) 
:
	_http_version(other._http_version),
	_status_code(other._status_code),
	_status_message(other._status_message),
	_headers(other._headers),
	_header_content_type(other._header_content_type),
	_header_content_length(other._header_content_length),
	_content_length(other._content_length),
	_body(other._body),
	_total_send_body(other._total_send_body),
	_location_server(other._location_server),
	_file_path(other._file_path),
	_client_socket(other._client_socket),
	_mime_types(other._mime_types),
	_mime_types_rev(other._mime_types_rev) 
	{}

Response &Response::operator=(const Response &other) {
	_http_version = other._http_version;
	_status_code = other._status_code;
	_status_message = other._status_message;
	_headers = other._headers;
	_header_content_type = other._header_content_type;
	_header_content_length = other._header_content_length;
	_content_length = other._content_length;
	_body = other._body;
	_total_send_body = other._total_send_body;
	_location_server = other._location_server;
	_file_path = other._file_path;
	_client_socket = other._client_socket;
	_mime_types = other._mime_types;
	_mime_types_rev = other._mime_types_rev;
	return *this;
}

Response::~Response() {}


void	Response::set_client_socket(int client_socket) {_client_socket = client_socket;}
void	Response::set_http_version(const std::string &http_version) {_http_version = http_version;}
void	Response::set_status_code(const std::string &status_code) {_status_code = status_code;}
void	Response::set_status_message(const std::string &status_message) {_status_message = status_message;}
void	Response::set_body(const std::string &body) {_body = body;}

void	Response::add_header(const std::string &header) {_headers += header + "\n"; }

std::string	Response::serialize_all() const {
	std::string response = _http_version + " " + _status_code + " " + _status_message + "\n" + _headers + "\n" + _body;
	return response;
}

std::string	Response::serialize_headers() const {
	std::string response = _http_version + " " + _status_code + " " + _status_message + "\n" + _headers + "\n";
	return response;
}

ssize_t Response::write_to_socket(const char *buffer, size_t size) const {
	ssize_t ret = write(_client_socket, buffer, size);
	cout << "DEBUG: write to socket return: " << ret << endl;
	if (ret == -1)
		std::cout << "[SERVER] Error writing to socket" << std::endl;
	return ret;
}

void  Response::set_header_content_type(const std::string &file_dir) {
	if (file_dir.find(".") != std::string::npos) {
		std::string ext = file_dir.substr(file_dir.find_last_of('.') + 1);
		
		std::map<std::string, std::string>::iterator it = _mime_types_rev.find(ext);
		if (it != _mime_types_rev.end()) {
			std::string extensions = it->second;
			_header_content_type = "Content-Type: " + extensions;
		} else {
			std::cout << "No mime type found for extension: " << ext << std::endl;
			_header_content_type = "Content-Type: text/html";
		}
		add_header(_header_content_type);
	}
}

void Response::set_header_content_length_file(std::ifstream &file) {
    if (!file.is_open()) {
        cout << "[SERVER]File is not open" << endl;
    }

    file.seekg(0, std::ios::end);

    if (file.fail()) {
        cout << "[SERVER]Failed to seek to end of file" << endl;
    }

    std::streamsize size = file.tellg();

    if (size == -1) {
       cout << "[SERVER]Failed to tell file size" << endl;
    }

    file.seekg(0, std::ios::beg);

    if (file.fail()) {
        cout << "[SERVER]Failed to seek to beginning of file" << endl;
    }

    _header_content_length = "Content-Length: " + std::to_string(size);
	cout << "DEBUG3" << endl;
	_content_length = (size >= 0) ? static_cast<size_t>(size) : 0;
	cout << "DEBUG4" << endl;

    add_header(_header_content_length);
}

void Response::set_content_from_file()
{
    std::string str;
    char buffer[BUFFER_SIZE_8K];
    std::streamsize ret;
	ret = _ifstream_response.read(buffer, BUFFER_SIZE_8K).gcount();
	_body.append(buffer, ret);
	if (ret < BUFFER_SIZE_8K)
		_ifstream_response.close();
}


void  Response::set_header_content_length_string(string &data) {
	_header_content_length = "Content-Length: ";
	std::stringstream ss;
	ss << data.size();
	_header_content_length += ss.str();
	_content_length = data.size();
	add_header(_header_content_length);
}

// ssize_t Response::body_send_all(int socket, const void *buffer, ssize_t length, int flags) {
	
// 	ssize_t sent = send(socket, (char*)buffer, length, flags);
// 	cout << "DEBUG: body_send_all() length input: " << length << endl;
// 	cout << "DEBUG: body_send_all() return send(): " << sent << endl;
// 	if (sent == -1) {
// 		// send would block, return how much was sent
// 		return -1;
// 	}


// 	return sent; // total_sent should be equal to length
// }
ssize_t Response::body_send_all(int socket, const void *buffer, ssize_t length, int flags) {
	ssize_t total_sent = _total_send_body;
	size_t buff_size;
	if (total_sent + BUFFER_SIZE_8K > length) {
		buff_size = length - total_sent;
	} else {
		buff_size = BUFFER_SIZE_8K;
	}
	if (total_sent < length) {
		
		ssize_t sent = send(socket, (char*)buffer + total_sent, buff_size, flags);
		cout << "DEBUG: write to socket return: " << sent << endl;
		if (sent == -1) {
			// send would block, return how much was sent
			_total_send_body = total_sent;
			return total_sent;
		}
		total_sent += sent;
	}
	_total_send_body = total_sent;
	return total_sent; // total_sent should be equal to length
}