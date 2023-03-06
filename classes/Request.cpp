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

std::vector<std::string>	Request::get_extention() const {
	std::vector<std::string> extention;

	std::map<std::string, std::vector<std::string> >::iterator it = _mime_types.find(_header_content_type);
	if (it != _mime_types.end()) {
			extention = it->second;
	} else {
			extention.push_back("html");
	}
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
