#include <string>

class Response {
private:
	std::string _http_version;
	std::string _status_code;
	std::string _status_message;
	std::string _headers;
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
	std::string	get_body() const;

	void	set_http_version(const std::string &http_version);
	void	set_status_code(const std::string &status_code);
	void	set_status_message(const std::string &status_message);
	void	set_headers(const std::string &headers);
	void	set_body(const std::string &body);

	std::string	serialize() const;
};
