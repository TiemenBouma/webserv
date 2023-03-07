#ifndef RESPONSE_HPP
#define RESPONSE_HPP


#include "typedef.h"
#include "Config.hpp"
#include <string>
#include <map>
#include <vector>
#include <sys/types.h>

class Response {
public:
	//MEMBERS NEED TO BE SET & SEND FOR RESPONSE
	std::string _http_version;
	std::string _status_code;
	std::string _status_message;
	std::string _headers;
	std::string _header_content_type;
	std::string _header_content_length;
	std::string _body;

	//MEMBERS FOR INTERNAL USE
	Location	*_location_serv;//relevant location on server
	string 		_file_path;	//path to file requested send back to client
	int			_client_socket; //socket to client where to send response

	//REFERENCE TO GLOBAL MIME TYPES
	map_str_vec_str & _mime_types;
	map_str_str  & _mime_types_rev;


	Response(map_str_vec_str &mime_types, map_str_str &mime_types_rev);
	Response(const Response &other);
	Response &operator=(const Response &other);
	~Response();

	void	set_client_socket(int client_socket);
	void	set_http_version(const std::string &http_version);
	void	set_status_code(const std::string &status_code);
	void	set_status_message(const std::string &status_message);
	void	set_body(const std::string &body);
	
	void	add_header(const std::string &header);//[INFO] adding single header to headers string
	
	void	set_header_content_type(const std::string &file_dir);//[INFO] deciding content type
	void	set_header_content_length(std::ifstream &file);//[INFO] calculating content length
	std::string	serialize_all() const; //general serialize
	std::string	serialize_headers() const; //for get request
	ssize_t write_to_socket(const char *buffer, size_t size) const;



};

#endif