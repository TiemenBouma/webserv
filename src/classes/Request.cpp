#include <iostream>
#include <sstream>
#include "Request.hpp"


Request::Request(map_str_vec_str &mime_types)
: _mime_types(mime_types)
{
	_valid_request = false;
	_error_log = "";
	_whole_request_at = 0;
	_content_length = 0;
	_state = REQUEST_START;
}

Request &Request::operator=(const Request &other) {
	_method = other._method;
	_url = other._url;
	_http_version = other._http_version;
	_headers = other._headers;
	_body = other._body;
	_header_content_type = other._header_content_type;
	_header_content_length = other._header_content_length;
	_valid_request = other._valid_request;
	_error_log = other._error_log;
	//_mime_types = other._mime_types;
	_whole_request_at = other._whole_request_at;
	_content_length = other._content_length;
	_state = other._state;
	return *this;
}

// [INFO] Constructor for the Request class does the initial parsing.
// Request::Request(std::stringstream & request_data, std::map<std::string, std::vector<std::string> > &mime_types, std::map<std::string, std::string>   &mime_types_rev) 
// : _mime_types(&mime_types)/*, _mime_types_rev(&mime_types_rev)*/ {
// 	_whole_request_at = 0;
// 	_content_length = 0;
// 	(void)mime_types_rev;
// 	// [INFO] Extract the method, URL, and version from the first line of the request
// 	request_data >> _method >> _url >> _http_version;

// 	// [INFO] Ensure the request is using HTTP/1.1
// 	if (_http_version != "HTTP/1.1") {
// 		_valid_request = false;
// 		_error_log += "Invalid request. Only HTTP/1.1 is supported.\n";
// 	}

// 	// [INFO] Extract the headers and body of the request
// 	std::string line;
// 	while (std::getline(request_data, line)) {
// 		if (line.empty()) {
// 			break;
// 		}	
// 		_headers += line + "\n";
// 	}

// 	//[CHECK] check if this IF ESLE is correct
// 	if (request_data.tellg() == LLONG_MAX || request_data.tellg() < 0) {
// 		_valid_request = true;
// 		_error_log +=  "No body in HTTP request.\n";
// 	}
// 	else if (static_cast<long long>(request_data.tellg()) < 
// 			static_cast<long long>(request_data.str().length()))
// 		_body = request_data.str().substr(request_data.tellg());
// 	else {
// 		_valid_request = false;
// 		_error_log +=  "Invalid request. tellg() out of range\n";
// 	}
// }

// [INFO] Extract the method, URL, and version from the first line of the request
void Request::set_method_url_version() {
	_state = REQUEST_READING_HEADERS;
	std::stringstream first_line(_whole_request.substr(0, _whole_request.find('\n')));
	first_line >> _method >> _url >> _http_version;
	_whole_request_at = _whole_request.find('\n') + 1;

	//cout << "[DEBUG] method: " << _method << endl;
	if (_method != "GET" && _method != "POST" && _method != "DELETE") {
		_valid_request = false;
		_error_log += "Invalid request. No method.\n";
		_state = REQUEST_CANCELLED;
	}
	if (_url.size() < 1) {
		_valid_request = false;
		_error_log += "Invalid request. No URL.\n";
		_state = REQUEST_CANCELLED;
	}
	if (_http_version != "HTTP/1.1") {
		_valid_request = false;
		_error_log += "Invalid request. Only HTTP/1.1 is supported.\n";
		_state = REQUEST_CANCELLED;
	}
}



//[INFO] Sort the headers into their respective variables, easier to add other headers. Just add if else.
void Request::set_headers() {
	if (_whole_request.find("\r\n\r\n") == std::string::npos) {
		_valid_request = false;
		_error_log += "Invalid request. No headers.\n";
		_state = REQUEST_CANCELLED;
		return ;
	}
	std::stringstream headers_stream(_whole_request.substr(_whole_request_at, _whole_request.find("\r\n\r\n")));
	_whole_request_at = _whole_request.find("\r\n\r\n", _whole_request_at) + 4;
	std::string line;
	while (std::getline(headers_stream, line)) {
		if (line.find("Content-Type") != std::string::npos) {
			_header_content_type = line.substr(line.find(":") + 2);
		}
		else if (line.find("Content-Length") != std::string::npos) {
			_header_content_length = line.substr(line.find(":") + 2);
			std::istringstream iss(_header_content_length);
			iss >> _content_length;
		}
	}
	_size_headers = _whole_request_at;
	_left_in_buff = _read_ret - _whole_request_at;
	_state = REQUEST_READING_BODY;

}

void Request::set_body() {
	_body += _whole_request.substr(_whole_request_at, _size_headers + _content_length);
	if (_whole_request.size() < _size_headers + _content_length) {
		_whole_request_at = _whole_request.size() + 1;
		_state = REQUEST_READING_BODY;
	} else {
		_whole_request_at = _size_headers + _content_length;
		_state = REQUEST_READING_DONE;
	}

}

std::vector<std::string>	Request::get_extention() const {
	std::vector<std::string> extention;
	std::string mime_type;
	mime_type = _header_content_type.substr(_header_content_type.find(":") + 2);
	
	std::map<std::string, std::vector<std::string> >::iterator it = _mime_types.find(mime_type);
	if (it != _mime_types.end()) {
			extention = it->second;
	} else {
			std::cout << "DEBUG: No extension found for: " << mime_type << std::endl;
			extention.push_back("html");
	}
	string extention = "." + extention[0];
	return extention;
}

std::string	Request::get_method() const {return _method;}
std::string	Request::get_path() const {return _url;} 
void		Request::set_path(const std::string &path) {_url = path;}
std::string	Request::get_http_version() const {return _http_version;} 
std::string	Request::get_headers() const {return _headers;} 
std::string	Request::get_body() const {return _body;} 

std::string	Request::get_content_type() const {return _header_content_type;}
std::string	Request::get_content_length() const {return _header_content_length;}

bool	Request::get_valid_request() const {return _valid_request;} 
std::string	Request::get_error_log() const {return _error_log;} 

std::ostream & operator<<(std::ostream &os, const Request &other) {
	os << other.get_method() << " " << other.get_path() << " " << other.get_http_version() 
		<< std::endl << other.get_headers() << "IS VALID:" << other.get_valid_request() << std::endl
		<< "ERROR LOG:" << other.get_error_log() << std::endl;
	return os;
}
