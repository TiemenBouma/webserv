#include "Connection.hpp"


Connection::Connection(ConfigServer &server, map_str_vec_str &mime_types, map_str_str &mime_types_rev) 
:	_port(server.listen_port), 
	_socket(-1),
	_request(Request(mime_types)),
	_resp(Response(mime_types, mime_types_rev)),
	_server(server) {}

Connection::Connection(const Connection &other) 
:	_port(other._port),
	_socket(other._socket),
	_request(other._request),
	_resp(other._resp),
	_server(other._server) {}

Connection &Connection::operator=(const Connection &other) {
	if (this != &other) {
		this->_port = other._port;
		this->_socket = other._socket;
		this->_request = other._request;
		this->_resp = other._resp;
	}
	return *this;
}

Connection::~Connection() {}
