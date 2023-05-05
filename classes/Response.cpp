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
	_body(""),
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
	_body(other._body),
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
	_body = other._body;
	_client_socket = other._client_socket;
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

void  Response::set_header_content_length_file(std::ifstream &file) {
	file.seekg(0, std::ios::end);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	
	_header_content_length = "Content-Length: ";
	std::stringstream ss;
	ss << size;
	_header_content_length += ss.str();
	add_header(_header_content_length);
}


void  Response::set_header_content_length_string(string &data) {
	_header_content_length = "Content-Length: ";
	std::stringstream ss;
	ss << data.size();
	_header_content_length += ss.str();
	add_header(_header_content_length);
}