#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <string>
#include <map>
#include <vector>
#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"

typedef std::map<std::string, std::vector<std::string > > map_str_vec_str;
typedef std::map<std::string, std::string> map_str_str;

//class Response;


class Connection {
public:
	Connection(ConfigServer &server, map_str_vec_str &mime_types, map_str_str &mime_types_rev);
	int					_port;
	int					_socket;
	Request				_request;
	Response			_resp;
	ConfigServer 		&_server;

};

#endif
