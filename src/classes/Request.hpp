#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "typedef.h"
#include <string>
#include <vector>
#include <map>
#include "Config.hpp"



enum request_state {
	REQUEST_START,
	REQUEST_READING_HEADERS,
	REQUEST_READING_BODY,
	REQUEST_READING_DONE,
	REQUEST_DONE,
	REQUEST_CANCELLED
};

class Request 
{
public:
	int			_state;
	//std::string	_buff_request;
	std::string	_whole_request;
	size_t		_whole_request_at;
	size_t		_left_in_buff;
	size_t		_size_headers;
	size_t		_content_length;
	size_t		_read_ret;
	Location	*_location_serv;
	Request(map_str_vec_str &mime_types);
	Request &operator=(const Request &other);
	void set_method(std::stringstream &req_stream);
	void set_method_url_version();
	void set_headers();
	void set_body();
	
	// Request(std::stringstream & request_data, std::map<std::string, std::vector<std::string> > &mime_types, 
	// 	std::map<std::string, std::string>   &mime_types_rev);

	std::vector<std::string>	get_extention() const ;
	

	std::string	get_method() const;
	std::string	get_path() const;
	void		set_path(const std::string &path);
	std::string	get_http_version() const;
	std::string	get_headers() const;
	std::string	get_body() const;

	std::string	get_content_type() const;
	std::string	get_content_length() const;

	bool	get_valid_request() const;
	std::string	get_error_log() const;

	std::string _error_log;
private:
	std::string _method;
	std::string _url;
	std::string _http_version;
	std::string _headers;
	std::string _body;

	std::string _header_content_type;
	std::string _header_content_length;

	map_str_vec_str &_mime_types;
	//std::map<std::string, std::string>  * _mime_types_rev;

	bool _valid_request;

};
std::ostream & operator<<(std::ostream &os, const Request &other);

#endif