#include "Cgi.hpp"

void	Cgi::exiterr(std::string e)
{
	perror(e.c_str());
	exit(0);
}

char**	Cgi::_make_exec_arg(std::string program, std::string path_info, std::string body)
{
	std::string	full_prog_path = path_info + program;
	char**	ret = new char*[3];

	ret[0] = strdup(full_prog_path.c_str());
	if (ret[0] == NULL)
		throw(CgiSystemFailure());
	ret[1] = strdup(body.c_str());
	if (ret[1] == NULL)
		throw(CgiSystemFailure());
	ret[2] = NULL;
	return (ret);
}

char**	Cgi::make_env(ConfigServer serv, Location loc, const std::string method)
{
	char**				env = new char*[5];
	char*				protocol = strdup("HTTP/1.1");
	std::stringstream	ss;
	ss << serv.listen_port;
	std::string			lstn_port_str = ss.str();

	std::cout << "[INFO] Making environment variables" << std::endl;
	env[SERVER] = const_cast<char *>(serv.server_name.c_str());
	env[SERVER_PROTOCOL] = protocol;
	env[SERVER_PORT] = const_cast<char *>(lstn_port_str.c_str());
	env[REQUEST_METHOD] = const_cast<char *>(method.c_str());
	env[PATH_INFO] = const_cast<char *>(loc.index.c_str());
	return (env);
}

std::string	Cgi::cgi(std::string program, char** env, std::string body)
{
	int			pid;
	char		buf[CGI_READ_SIZE];
	int			readret;
	std::string	ret;
	int			fds[2];
	int			status;

	if (pipe(fds) == -1)
		throw(CgiSystemFailure());
	if ((pid = fork()) == -1)
		throw(CgiSystemFailure());
	if (pid == 0)
	{
		char**	args = _make_exec_arg(program, "path_info", body);

		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		//write(fds[1], &body, size);
		execve(args[0], args, env);
		throw(CgiSystemFailure());
	}

	wait(&status);
	close(fds[1]);
	struct pollfd poll_fd = {fds[0], POLLIN, 0};
	while (1)
	{
		poll(&poll_fd, sizeof(struct pollfd), 0);
		readret = read(fds[0], buf, CGI_READ_SIZE);
		if (readret == -1)
			throw(CgiSystemFailure());
		if (readret == 0)
			break;
		ret += std::string(buf, readret);
	}
	close(fds[0]);
	return (ret);
}
