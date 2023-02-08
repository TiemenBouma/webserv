#include "Config.hpp"

ConfigServer::ConfigServer(){}
ConfigServer::ConfigServer(ConfigServer const &other) {
	*this = other;
}
ConfigServer &ConfigServer::operator=(ConfigServer const &other) {
	if (this != &other) {
		this->listen_port = other.listen_port;
		this->server_name = other.server_name;
		this->error_pages = other.error_pages;
		this->max_size_content = other.max_size_content;
		this->root = other.root;
		this->cgi_path = other.cgi_path;
		this->redir_src = other.redir_src;
		this->redir_dst = other.redir_dst;
		this->locations = other.locations;
		this->server_soc = other.server_soc;
	}
	return *this;
}
ConfigServer::~ConfigServer(){}

// std::vector<std::string> tokenize(std::string conf) {
// 	std::string::size_type 		end;
// 	std::vector<std::string>	tokens;

// 	for(std::string::size_type i = 0; i < conf.length();)
// 	{
// 		/*special characters*/
// 		if (conf[i] == '{' || conf[i] == ';' || conf[i] == '}') {
// 			std::string token(&conf[i], 1);
// 			tokens.push_back(token);
// 			i++;
// 			continue;
// 		/*whitespace*/
// 		} else if (conf[i] == ' ' || conf[i] == '\n' || conf[i] == '\t') {
// 			i++;
// 			continue;
// 		/*comments*/
// 		} else if (conf[i] == '#') {
// 			end = conf.find_first_of("\n", i);
// 			i = end;
// 		/*other strings*/
// 		} else {
// 			end = conf.find_first_of(" \t\n;{}", i);
// 			if(end == std::string::npos)
// 			{
// 				end = conf.length();
// 			}
// 			std::string token(&conf[i], end-i);
// 			tokens.push_back(token);
// 			i = end;
// 		}
// 	}
//    return tokens;
// }