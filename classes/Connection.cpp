#include "Connection.hpp"


Connection::Connection(ConfigServer &server, map_str_vec_str &mime_types, map_str_str &mime_types_rev) 
:	_port(server.listen_port), 
	_socket(-1),
	_request(Request(mime_types)),
	_response(Response(mime_types, mime_types_rev)),
	_server(server) {}

Connection::Connection(const Connection &other) 
:	_port(other._port),
	_socket(other._socket),
	_request(other._request),
	_response(other._response),
	_server(other._server) {}

Connection &Connection::operator=(const Connection &other) {
	if (this != &other) {
		this->_port = other._port;
		this->_socket = other._socket;
		this->_request = other._request;
		this->_response = other._response;
	}
	return *this;
}

Connection::~Connection() {}


void Connection::set_location() {

	Location loc;
	string uri = _request.get_url();
	_response._location_serv = NULL;

	for (size_t i = 0; i < _server.locations.size(); i++) {
		if (_server.locations[i].location == uri) {
			_response._location_serv = &_server.locations[i];
			_response._file_path = _server.root + _response._location_serv->index;
			return;
		}
	}

	_response._status_code = "404";
	_response.set_status_message("not found");

	//maybe send error page here?
}

int	Connection::check_method() {

	for (size_t i = 0; i < _response._location_serv->accepted_methods.size(); i++) {
		if (_response._location_serv->accepted_methods[i] == _request.get_method())
			return 0;
	}
	_response._status_code = "405";
	_response.set_status_message("method not allowed");
	//Maybe send errorpage here?
	return 1;
}