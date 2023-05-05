#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"
#include "typedef.h"
#include <string>
#include <map>
#include <vector>




//class Response;
class Connection {
public:
	int					_port;
	int					_socket;
	Request				_request;
	Response			_response;
	ConfigServer 		&_server;
	time_t				_last_active;
	
	Connection();
	Connection(const Connection &other);
	Connection &operator=(const Connection &other);
	~Connection();
	Connection(ConfigServer &server, map_str_vec_str &mime_types, map_str_str &mime_types_rev);

	void set_location();
	int	check_method();

	/*returns true if status is error*/
	bool check_response_status_error();

	int check_time_out();
	void set_method_url_version();
	void reading_headers();
	void set_headers();
	void set_body();

};

#endif
