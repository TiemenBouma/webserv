#include <iostream>
#include <sstream>
#include "Request.hpp"


Request::Request(map_str_vec_str &mime_types)
: _mime_types(mime_types)
{
	_valid_request = false;
	//_error_log = "";
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
	//_error_log = other._error_log;
	//_mime_types = other._mime_types;
	_whole_request_at = other._whole_request_at;
	_content_length = other._content_length;
	_state = other._state;
	return *this;
}

// [INFO] Extract the method, URL, and version from the first line of the request
void Request::set_method_url_version() {
	std::stringstream first_line(_whole_request.substr(0, _whole_request.find('\n')));
	first_line >> _method >> _url >> _http_version;
	_whole_request_at = _whole_request.find('\n') + 1;

	if (_method != "GET" && _method != "POST" && _method != "DELETE") {
		_valid_request = false;
		_state = REQUEST_CANCELLED;
		return;
	}
	if (_url.size() < 1) {
		_valid_request = false;
		_state = REQUEST_CANCELLED;
		return;
	}
	//which HTTP versions do we accept?
	if (_http_version != "HTTP/1.1") {
		_valid_request = false;
		_state = REQUEST_CANCELLED;
		return;
	}
	_state = REQUEST_READING_HEADERS;
}

void Request::reading_headers() {

	if (_whole_request.find("\r\n\r\n") == std::string::npos) {
		if (_whole_request.size() >= BUFFER_SIZE_8K)
			_state = REQUEST_CANCELLED;
		return ;
	}
	if (_whole_request.find("\r\n\r\n") != std::string::npos) {
		_state = REQUEST_SETTING_HEADERS;
	}
}

//[INFO] Sort the headers into their respective variables, easier to add other headers. Just add if else.
void Request::set_headers() {

	std::stringstream headers_stream(_whole_request.substr(_whole_request_at, _whole_request.find("\r\n\r\n")));
	_whole_request_at = _whole_request.find("\r\n\r\n", _whole_request_at) + 4;
	std::string line;
	while (std::getline(headers_stream, line)) {
		if (line.find("Content-Type") != std::string::npos) {
			_header_content_type = line.substr(line.find(":") + 2);
			_header_content_type = _header_content_type.substr(0, _header_content_type.size() - 1); // INFO: minus 1 because last char is \r I think
		}
		else if (line.find("Content-Length") != std::string::npos) {
			_header_content_length = line.substr(line.find(":") + 2);
			std::istringstream iss(_header_content_length);
			iss >> _content_length;
		}
	}
	_headers_length = _whole_request_at;
	//_left_in_buff = _read_ret - _whole_request_at;
	_state = REQUEST_READING_BODY;
}

void Request::set_body() {
	_body += _whole_request.substr(_whole_request_at, _headers_length + _content_length);
	if (_whole_request.size() < _headers_length + _content_length) {
		_whole_request_at = _whole_request.size() + 1;
		_state = REQUEST_READING_BODY;
	} else {
		_whole_request_at = _headers_length + _content_length;
		_state = REQUEST_READING_DONE;
	}

}

std::vector<std::string>	Request::get_extention() const {
	std::vector<std::string> extention;

	std::map<std::string, std::vector<std::string> >::iterator it = _mime_types.find(_header_content_type);
	if (it != _mime_types.end()) {
			extention = it->second;
	} else {
		//	std::cout << "DEBUG: No extension found for: " << _header_content_type << std::endl;
			extention.push_back("html");
	}
	//string extention2 = "." + extention[0];
	return extention;
}

std::string	Request::get_method() const {return _method;}
std::string	Request::get_url() const {return _url;} 
void		Request::set_url(const std::string &url) {_url = url;}
std::string	Request::get_http_version() const {return _http_version;} 
std::string	Request::get_headers() const {return _headers;} 
std::string	Request::get_body() const {return _body;} 

std::string	Request::get_content_type() const {return _header_content_type;}
std::string	Request::get_content_length() const {return _header_content_length;}

bool	Request::get_valid_request() const {return _valid_request;} 
//std::string	Request::get_error_log() const {return _error_log;} 

std::ostream & operator<<(std::ostream &os, const Request &other) {
	os << other.get_method() << " " << other.get_url() << " " << other.get_http_version() 
		<< std::endl << other.get_headers() << "IS VALID:" << other.get_valid_request() << std::endl
		<< "ERROR LOG:" <<  std::endl;
	return os;
}
