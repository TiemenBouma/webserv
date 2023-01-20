#include <string>


class Request 
{
public:
	Request(const char * request);

	void sort_headers();
	

	std::string	get_method() const;
	std::string	get_url() const;
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

	bool _valid_request;
	std::string _error_log;

};
std::ostream & operator<<(std::ostream &os, const Request &other);