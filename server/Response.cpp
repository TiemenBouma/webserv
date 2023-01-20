#include "Response.hpp"
#include "Request.hpp"

Response::Response() {
	_http_version = "HTTP/1.1";
	_status_code = "200";
	_status_message = "OK";
	_headers = "Content-Type: text/html";
	_body = "";
}

Response::Response(const Response &other) {
	_http_version = other.get_http_version();
	_status_code = other.get_status_code();
	_status_message = other.get_status_message();
	_headers = other.get_headers();
	_body = other.get_body();
}

Response &Response::operator=(const Response &other) {
	_http_version = other.get_http_version();
	_status_code = other.get_status_code();
	_status_message = other.get_status_message();
	_headers = other.get_headers();
	_body = other.get_body();
	return *this;
}

Response::~Response() {}

std::string	Response::get_http_version() const {return _http_version;}
std::string	Response::get_status_code() const {return _status_code;}
std::string	Response::get_status_message() const {return _status_message;}
std::string	Response::get_headers() const {return _headers;}
std::string	Response::get_body() const {return _body;}

void	Response::set_http_version(const std::string &http_version) {_http_version = http_version;}
void	Response::set_status_code(const std::string &status_code) {_status_code = status_code;}
void	Response::set_status_message(const std::string &status_message) {_status_message = status_message;}
void	Response::set_headers(const std::string &header) {_headers += "\r\n" + header;}
void	Response::set_body(const std::string &body) {_body = body;}

std::string	Response::serialize() const {
	std::string response = _http_version + " " + _status_code + " " + _status_message + "\r\n" + _headers + "\r\n\r\n" + _body;
	return response;
}

void Response::parse_request(Request &req) {
	std::string url = req.get_url();
	std::string method = req.get_method();
	std::string body = req.get_body();
	std::string headers = req.get_headers();
	std::string http_version = req.get_http_version();
	// std::string host = req.get_host();
	// std::string accept = req.get_accept();
	// std::string accept_encoding = req.get_accept_encoding();
	// std::string accept_language = req.get_accept_language();
	// std::string connection = req.get_connection();
	// std::string user_agent = req.get_user_agent();
	std::string content_length = req.get_content_length();
	std::string content_type = req.get_content_type();
	// std::string transfer_encoding = req.get_transfer_encoding();
	// std::string upgrade_insecure_requests = req.get_upgrade_insecure_requests();
	// std::string cookie = req.get_cookie();
	// std::string location = req.get_location();
	// std::string server = req.get_server();
	// std::string date = req.get_date();
	// std::string last_modified = req.get_last_modified();
	// std::string content_encoding = req.get_content_encoding();
	// std::string content_language = req.get_content_language();
	// std::string content_location = req.get_content_location();
	// std::string content_md5 = req.get_content_md5();
	// std::string content_range = req.get_content_range();
	// std::string expires = req.get_expires();
	// std::string retry_after = req.get_retry_after();
	// std::string vary = req.get_vary();
	// std::string warning = req.get_warning();
	// std::string allow = req.get_allow();
	// std::string content_disposition = req.get_content_disposition();
	// std::string content_security_policy = req.get_content_security_policy();
	// std::string content_security_policy_report_only = req.get_content_security_policy_report_only();
	// std::string public_key_pins = req.get_public_key_pins();
	// std::string public_key_pins_report_only = req.get_public_key_pins_report_only();
	// std::string strict_transport_security = req.get_strict_transport_security();
	// std::string x_content_type_options = req.get_x_content_type_options();
	// std::string x_frame_options = req.get_x_frame_options();
	// std::string x_xss_protection = req.get_x_xss_protection();
	// std::string x_powered_by = req.get_x_powered_by();
	// std::string x_ua_compatible = req.get_x_ua_compatible();
	// std::string x_content_security_policy = req.get_x_content_security_policy();
	// std::string x_webkit
}

