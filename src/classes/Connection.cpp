#include "Connection.hpp"
#include "Response.hpp"


Connection::Connection(ConfigServer &server, map_str_vec_str &mime_types, map_str_str &mime_types_rev) 
	: _server(server)
{
	_port = server.listen_port;
	_socket = -1;
	_request = Request();	
	_resp = Response(mime_types, mime_types_rev);
}