#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include "Request.hpp"

class Response {
private:
	std::string _http_version;
	std::string _status_code;
	std::string _status_message;
	std::string _headers;
	std::string _header_content_type;
	std::string _header_content_length;
	std::string _body;
public:
	Response();
	Response(const Response &other);
	Response &operator=(const Response &other);
	~Response();

	std::string	get_http_version() const;
	std::string	get_status_code() const;
	std::string	get_status_message() const;
	std::string	get_headers() const;
	std::string	get_header_content_type() const;
	std::string	get_header_content_length() const;
	std::string	get_body() const;

	void	set_http_version(const std::string &http_version);
	void	set_status_code(const std::string &status_code);
	void	set_status_message(const std::string &status_message);
	void	add_header(const std::string &header);
	void	set_body(const std::string &body);
	//I want to decide what the content type is
	void	set_header_content_type(const std::string &file_dir);
	//I want to decide what the content length is
	void	set_header_content_length(size_t header_content_length);


	std::string	serialize() const;

	void parse_request(Request &req);
};

#endif