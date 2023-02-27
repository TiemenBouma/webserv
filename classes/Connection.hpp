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
	Connection();
	Connection(const Connection &other);
	Connection &operator=(const Connection &other);
	~Connection();
	Connection(ConfigServer &server, map_str_vec_str &mime_types, map_str_str &mime_types_rev);
	int					_port;
	int					_socket;
	Request				_request;
	Response			_resp;
	ConfigServer 		&_server;

};

#endif
