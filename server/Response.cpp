#include "Response.hpp"

Response::Response() {
	_http_version = "HTTP/1.1";
	_status_code = "200";
	_status_message = "OK";
	_headers = "Content-Type: text/html\r\n";
	_body = "";
}

Response::Response(const Response &other) {
	_http_version = other.get_http_version();
	_status_code = other.get_status_code();
	_status_message = other.get_status_message();
	_headers = other.get_headers();
	_body = other.get_body();
}

Response &Response::operator=(const Response &other) {
	_http_version = other.get_http_version();
	_status_code = other.get_status_code();
	_status_message = other.get_status_message();
	_headers = other.get_headers();
	_body = other.get_body();
	return *this;
}

Response::~Response() {}

std::string	Response::get_http_version() const {return _http_version;}
std::string	Response::get_status_code() const {return _status_code;}
std::string	Response::get_status_message() const {return _status_message;}
std::string	Response::get_headers() const {return _headers;}
std::string	Response::get_body() const {return _body;}

void	Response::set_http_version(const std::string &http_version) {_http_version = http_version;}
void	Response::set_status_code(const std::string &status_code) {_status_code = status_code;}
void	Response::set_status_message(const std::string &status_message) {_status_message = status_message;}
void	Response::set_headers(const std::string &header) {_headers += "\n" + header;}
void	Response::set_body(const std::string &body) {_body = body;}

std::string	Response::serialize() const {
	std::string response = _http_version + " " + _status_code + " " + _status_message + "\r\n" + _headers + "\r\n" + _body;
	return response;
}

