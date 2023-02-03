#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <string>
#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"

class Connection {
public:
	Connection(ConfigServer &server);
	int					_port;
	int					_socket;
	Request				_request;
	Response			_resp;
	ConfigServer 		&_server;

};

#endif