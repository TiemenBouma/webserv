#include <iostream>
#include <sstream>
#include <string>
#include <climits>
#include "Request.hpp"

Request::Request(const char* request) {

	std::stringstream request_stream(request);

	// Extract the method, URL, and version from the first line of the request
	request_stream >> _method >> _url >> _http_version;

	// Ensure the request is using HTTP/1.1
	if (_http_version != "HTTP/1.1") {
		_valid_request = false;
		_error_log += "Invalid request. Only HTTP/1.1 is supported.\n";
	}

	// Extract the headers and body of the request
	std::string line;
	while (std::getline(request_stream, line)) {
		if (line.empty()) {
			break;
		}	
		_headers += line + "\n";
	}
	if (request_stream.tellg() == LLONG_MAX || request_stream.tellg() < 0) {
		_valid_request = true;
		_error_log +=  "No body in HTTP request.\n";
	}
	else if (static_cast<long long>(request_stream.tellg()) < static_cast<long long>(request_stream.str().length()))
		_body = request_stream.str().substr(request_stream.tellg());
	else {
		_valid_request = false;
		_error_log +=  "Invalid request. tellg() out of range\n";
	}
}

std::string	Request::get_method() const {return _method;}
std::string	Request::get_url() const {return _url;} 
std::string	Request::get_http_version() const {return _http_version;} 
std::string	Request::get_headers() const {return _headers;} 
std::string	Request::get_body() const {return _body;} 
bool	Request::get_valid_request() const {return _valid_request;} 
std::string	Request::get_error_log() const {return _error_log;} 

std::ostream & operator<<(std::ostream &os, const Request &other) {
	os << other.get_method() << " " << other.get_url() << " " << other.get_http_version() 
		<< std::endl << other.get_headers() << "IS VALID:" << other.get_valid_request() << std::endl
		<< "ERROR LOG:" << other.get_error_log() << std::endl;
	return os;
}
