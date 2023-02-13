#ifndef RESPONSE_HPP
#define RESPONSE_HPP


#include "typedef.h"
#include "Config.hpp"
#include <string>
#include <map>
#include <vector>

class Response {
public:
	Location	*_location_serv;
	string 		_file_path;	//path to file
	int _client_socket;

	std::string _http_version;
	std::string _status_code;
	std::string _status_message;
	std::string _headers;
	std::string _header_content_type;
	std::string _header_content_length;
	std::string _body;
	map_str_vec_str & _mime_types;
	map_str_str  & _mime_types_rev;


private:

public:
	Response(map_str_vec_str &mime_types, map_str_str &mime_types_rev);
	Response(const Response &other);
	Response &operator=(const Response &other);
	~Response();

	int			get_client_socket() const;
	std::string	get_http_version() const;
	std::string	get_status_code() const;
	std::string	get_status_message() const;
	std::string	get_headers() const;
	std::string	get_header_content_type() const;
	std::string	get_header_content_length() const;
	std::string	get_body() const;
	std::map <std::string, std::vector<std::string> > get_mime_types() const;

	void	set_client_socket(int client_socket);
	void	set_http_version(const std::string &http_version);
	void	set_status_code(const std::string &status_code);
	void	set_status_message(const std::string &status_message);
	void	add_header(const std::string &header);
	void	set_body(const std::string &body);
	//[INFO] deciding content type
	void	set_header_content_type(const std::string &file_dir);
	//[INFO] calculating content length
	void	set_header_content_length(std::ifstream &file);


	std::string	serialize() const; //general serialize
	std::string	serialize_headers() const; //for get request
	void write_to_socket(const char *buffer, size_t size) const;

	//void parse_request(Request &req);
};

#endif