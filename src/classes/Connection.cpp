#include "Connection.hpp"


Connection::Connection(ConfigServer &server) 
	: _server(server)
{
	_port = server.listen_port;
	_socket = -1;
	_request = Request();	
	_resp = Response();
	
}