#include "Connection.hpp"
#include <iostream>
#include <sstream>


Connection::Connection(ConfigServer &server, map_str_vec_str &mime_types, map_str_str &mime_types_rev) 
:	_port(server.listen_port), 
	_socket(-1),
	_request(Request(mime_types)),
	_response(Response(mime_types, mime_types_rev)),
	_server(server) {
	_last_active = time(NULL);
	}

Connection::Connection(const Connection &other) 
:	_port(other._port),
	_socket(other._socket),
	_request(other._request),
	_response(other._response),
	_server(other._server),
	_last_active(other._last_active) {}

Connection &Connection::operator=(const Connection &other) {
	if (this != &other) {
		this->_port = other._port;
		this->_socket = other._socket;
		this->_request = other._request;
		this->_response = other._response;
		this->_last_active = other._last_active;
	}
	return *this;
}

Connection::~Connection() {}

//CHECK THIS FUNCTION, redir comparion not working.
void Connection::set_location() {

	Location loc;
	string uri = _request.get_url();
	if (_response._location_server != NULL)
		return;

	for (size_t i = 0; i < _server.locations.size(); i++) {
		if (_server.locations[i].location == uri) {
			_response._location_server = &_server.locations[i];
			if (_response._location_server->redir.begin() == _response._location_server->redir.end()){
				_response._file_path = _server.root + _response._location_server->index;

			}
			else {
				_response._file_path = _server.root + _response._location_server->redir[301];
				_response._status_code = "301";
			}
			return;
		}
	}

	_response._status_code = "404";
	_response.set_status_message("not found");

	//maybe send error page here?
}

bool Connection::check_response_status_error() {
	if (_response._status_code == "200")
		return 0;
	if (_response._status_code == "301")
		return 0;
	return 1;
}

int	Connection::check_method() {

	for (size_t i = 0; i < _response._location_server->accepted_methods.size(); i++) {
		if (_response._location_server->accepted_methods[i] == _request.get_method())
			return 0;
	}
	_response._status_code = "405";
	_response.set_status_message("method not allowed");
	//Maybe send errorpage here?
	return 1;
}

int Connection::check_time_out() {
	const int TIMEOUT_THRESHOLD = 10;
	time_t current_time = time(NULL);

	if (current_time - _last_active > TIMEOUT_THRESHOLD) {
		cout << "[SERVER] closing idle connection" << _request.get_method() << " " << _request.get_url() << endl;
		_response._status_code = "408";
		_response.set_status_message("request timeout");
		_request._state = REQUEST_CANCELLED;
		return 1;
		
	}
	return 0;
}

// [INFO] Extract the method, URL, and version from the first line of the request
// sets errorcode if someting goes wrong.
void Connection::set_method_url_version() {
	std::stringstream first_line(_request._whole_request.substr(0, _request._whole_request.find('\n')));
	first_line >> _request._method >> _request._url >> _request._http_version;
	_request._whole_request_at = _request._whole_request.find('\n') + 1;

	if (_request._method != "GET" && _request._method != "POST" && _request._method != "DELETE") {
		_request._valid_request = false;
		_request._state = REQUEST_CANCELLED;
		_response._status_code = "400";
		_response.set_status_message("bad request");
		return;
	}
	if (_request._url.size() < 1) {
		_request._valid_request = false;
		_request._state = REQUEST_CANCELLED;
		_response._status_code = "400";
		_response.set_status_message("bad request");
		return;
	}
	//which HTTP versions do we accept?
	if (_request._http_version != "HTTP/1.1") {
		_request._valid_request = false;
		_request._state = REQUEST_CANCELLED;
		_response._status_code = "400";
		_response.set_status_message("bad request");
		return;
	}
	_request._state = REQUEST_READING_HEADERS;
}

void Connection::reading_headers() {

	if (_request._whole_request.find("\r\n\r\n") == std::string::npos) {
		if (_request._whole_request.size() >= BUFFER_SIZE_8K) {
			_request._state = REQUEST_CANCELLED;
			_response._status_code = "431";
			_response.set_status_message("request header fields too large");
		}
		return ;
	}
	if (_request._whole_request.find("\r\n\r\n") != std::string::npos) {
		_request._headers_length = _request._whole_request.find("\r\n\r\n") + 4;
		_request._state = REQUEST_SETTING_HEADERS;
	}
}

//[INFO] Sort the headers into their respective variables, easier to add other headers. Just add if else.

void Connection::set_headers() {

	std::stringstream headers_stream(_request._whole_request.substr(_request._whole_request_at, _request._whole_request.find("\r\n\r\n")));
	_request._whole_request_at = _request._whole_request.find("\r\n\r\n", _request._whole_request_at) + 4;
	std::string line;
	while (std::getline(headers_stream, line)) {
		if (line.find("Content-Type") != std::string::npos) {
			_request._header_content_type = line.substr(line.find(":") + 2);
			_request._header_content_type = _request._header_content_type.substr(0, _request._header_content_type.size() - 1); // INFO: minus 1 because last char is \r I think
		}
		else if (line.find("Content-Length") != std::string::npos) {
			_request._header_content_length = line.substr(line.find(":") + 2);
			std::istringstream iss(_request._header_content_length);
			iss >> _request._content_length;
		}
	}
	if (_request._content_length == 0)
		_request._state = REQUEST_DONE;
	else
		_request._state = REQUEST_READING_BODY;
}

void Connection::set_body() {
	_request._body += _request._whole_request.substr(_request._whole_request_at);
	if (_request._whole_request.size() < _request._headers_length + _request._content_length) {
		_request._whole_request_at = _request._whole_request.size() + 1;
	} 
	else if (_request._whole_request.size() == _request._headers_length + _request._content_length) {
		_request._whole_request_at = _request._whole_request.size() + 1;
		_request._state = REQUEST_DONE;
	}
	else {
		_request._state = REQUEST_CANCELLED;
		_response._status_code = "400";
		_response.set_status_message("bad request");
	}
}

// void Connection::calc_req_state() {

// 	if (_request._state == REQUEST_DONE && _response._body.size() == _response._total_send_body)
// 	_request._state = REQUEST_DONE_AND_SEND;
// }