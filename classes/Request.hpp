#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "typedef.h"
#include <string>
#include <vector>
#include <map>
#include "Config.hpp"
#include <ctime>

enum request_state {
	REQUEST_START,
	REQUEST_READING_HEADERS,
	REQUEST_SETTING_HEADERS,
	REQUEST_READING_BODY,
	REQUEST_READING_DONE,
	REQUEST_DONE,
	REQUEST_CANCELLED
};

class Request 
{
public:

	//CONSTRUCTORS
	Request(map_str_vec_str &mime_types);
	Request &operator=(const Request &other);

	//SETTERS

	void set_body();

	//GETTERS
	std::vector<std::string>	get_extention() const ;
	std::string	get_method() const;
	std::string	get_url() const;
	void		set_url(const std::string &url);
	std::string	get_http_version() const;
	std::string	get_headers() const;
	std::string	get_body() const;
	std::string	get_content_type() const;
	std::string	get_content_length() const;
	bool		get_valid_request() const;

	//MEMBERS (PUBLIC dynamic for parsing request)
	int			_state;
	std::string	_whole_request;
	size_t		_whole_request_at;
	size_t		_headers_length;
	size_t		_content_length;
	Location	*_location_serv;
	bool _valid_request;
	time_t		_start_time;



	//MEMBERS (set once after parsing request)
	std::string _method;
	std::string _url;
	std::string _http_version;
	std::string _headers;
	std::string _body;
	std::string _header_content_type;
	std::string _header_content_length;

	//REFFERENCE TO GLOBAL MIME TYPES
	map_str_vec_str &_mime_types;


};
std::ostream & operator<<(std::ostream &os, const Request &other);

#endif