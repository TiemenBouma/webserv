#include <iostream>
#include <sstream>
#include "Request.hpp"


Request::Request(map_str_vec_str &mime_types)
: _mime_types(mime_types)
{
	_state = 0;
	_whole_request = "";
	_whole_request_at = 0;
	_header_content_length = "";
	_content_length = 0;
	_location_server = NULL;
	_valid_request = false;
	time(&_start_time);

	_method = "";
	_url = "";
	_http_version = "";
	_headers = "";
	_body = "";
	_header_content_type = "";
	_header_content_length = "";
	_req_recieved = false;


}

Request &Request::operator=(const Request &other) {
	_state = other._state;
	_whole_request = other._whole_request;
	_whole_request_at = other._whole_request_at;
	_header_content_length = other._header_content_length;
	_content_length = other._content_length;
	_location_server = other._location_server;
	_valid_request = other._valid_request;
	_start_time = other._start_time;

	_method = other._method;
	_url = other._url;
	_http_version = other._http_version;
	_headers = other._headers;
	_body = other._body;
	_header_content_type = other._header_content_type;
	_header_content_length = other._header_content_length;
	_mime_types = other._mime_types;
	_req_recieved = other._req_recieved;
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
