#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <vector>
#include <map>

enum request_state {
	REQUEST_START,
	REQUEST_PROCESSING,
	REQUEST_WRITING,
	REQUEST_DONE,
	REQUEST_CANCELLED
};

class Request 
{
public:
	int			state;
	std::string	whole_request;
	Request();
	
	Request(std::stringstream & request_data, std::map<std::string, std::vector<std::string> > &mime_types, 
		std::map<std::string, std::string>   &mime_types_rev);

	void sort_headers();
	std::vector<std::string>	get_extention() const ;
	

	std::string	get_method() const;
	std::string	get_path() const;
	std::string	get_http_version() const;
	std::string	get_headers() const;
	std::string	get_body() const;

	std::string	get_content_type() const;
	std::string	get_content_length() const;

	bool	get_valid_request() const;
	std::string	get_error_log() const;

private:
	std::string _method;
	std::string _url;
	std::string _http_version;
	std::string _headers;
	std::string _body;

	std::string _header_content_type;
	std::string _header_content_length;

	std::map<std::string, std::vector<std::string> > *_mime_types;
	//std::map<std::string, std::string>  * _mime_types_rev;

	bool _valid_request;
	std::string _error_log;

};
std::ostream & operator<<(std::ostream &os, const Request &other);

#endif