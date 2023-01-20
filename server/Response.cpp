#include "Response.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

Response::Response() {
	_http_version = "HTTP/1.1";
	_status_code = "200";
	_status_message = "OK";
	_headers = "";
	_body = "";
}

Response::Response(const Response &other) {
	_http_version = other.get_http_version();
	_status_code = other.get_status_code();
	_status_message = other.get_status_message();
	_headers = other.get_headers();
	_header_content_length = other.get_header_content_length();
	_header_content_type = other.get_header_content_type();
	_body = other.get_body();
}

Response &Response::operator=(const Response &other) {
	_http_version = other.get_http_version();
	_status_code = other.get_status_code();
	_status_message = other.get_status_message();
	_headers = other.get_headers();
	_header_content_length = other.get_header_content_length();
	_header_content_type = other.get_header_content_type();
	_body = other.get_body();
	return *this;
}

Response::~Response() {}

std::string	Response::get_http_version() const {return _http_version;}
std::string	Response::get_status_code() const {return _status_code;}
std::string	Response::get_status_message() const {return _status_message;}
std::string	Response::get_headers() const {return _headers;}
std::string	Response::get_header_content_type() const {return _header_content_type;}
std::string	Response::get_header_content_length() const {return _header_content_length;}
std::string	Response::get_body() const {return _body;}

void	Response::set_http_version(const std::string &http_version) {_http_version = http_version;}
void	Response::set_status_code(const std::string &status_code) {_status_code = status_code;}
void	Response::set_status_message(const std::string &status_message) {_status_message = status_message;}
void	Response::add_header(const std::string &header) {_headers += header + "\r\n"; }
void	Response::set_body(const std::string &body) {_body = body;}

std::string	Response::serialize() const {
	std::string response = _http_version + " " + _status_code + " " + _status_message + "\n" + _headers + "\n" + _body;
	return response;
}

void  Response::set_header_content_type(const std::string &file_dir) {
	if (file_dir.find(".") != std::string::npos) {

		std::string ext = file_dir.substr(file_dir.find(".", 3) + 1);
		std::ifstream extentions;
		extentions.open("../data/content_type.txt", std::ifstream::in);
		if (extentions.fail()) 
			std::cout << "Error opening file" << std::endl;
		std::string line;
		while (getline(extentions, line)) {
			if (line.find(ext) != std::string::npos) {
				_header_content_type = "Content-Type: " + line;
				add_header(_header_content_type);
				extentions.close();
				return ;
			}
		}
	}
	else {
		_header_content_type += "Content-Type: text/html";
		add_header(_header_content_type);
		return ;
	}
}

void  Response::set_header_content_length(size_t header_content_length) {
	_header_content_length = "Content-Length: ";
	std::stringstream ss;
	ss << header_content_length;
	_header_content_length += ss.str();
	add_header(_header_content_length);
}
