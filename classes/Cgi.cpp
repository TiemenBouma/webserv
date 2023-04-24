#include "Cgi.hpp"

void	Cgi::exiterr(std::string e)
{
	perror(e.c_str());
	exit(0);
}

char**	Cgi::_make_exec_arg(std::string program, std::string body)
{
	std::string	full_prog_path = program;
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
	std::stringstream	ss;
	ss << serv.listen_port;
	std::string			lstn_port_str = ss.str();

	env[SERVER] = strdup(("SERVER_NAME=" + serv.server_name).c_str());
	env[SERVER_PROTOCOL] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	env[SERVER_PORT] = strdup(("SERVER_PORT=" + lstn_port_str).c_str());
	env[REQUEST_METHOD] = strdup(("REQUEST_METHOD=" + method).c_str());
	env[PATH_INFO] = strdup(("PATH_INFO=" + loc.index).c_str());
	env[NULL_END] = NULL;
	return (env);
}

void	print_env(char **env)
{
	for (int i = 0; env[i] != NULL; i++)
	{
		std::cout << env[i] << std::endl;
	}
}

std::string	Cgi::cgi(std::string program, char** env, std::string body)
{
	int			pid;
	char		buf[CGI_READ_SIZE];
	int			readret;
	std::string	ret;
	int			fds[2];
	int			std_out_cpy;
	int			status;

	std_out_cpy = dup(STDOUT_FILENO);
	if (pipe(fds) == -1)
		throw(CgiSystemFailure());
	if ((pid = fork()) == -1)
		throw(CgiSystemFailure());
	if (pid == 0)
	{
		char**	args = _make_exec_arg(program, body);

		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		//write(fds[1], &body, size);
		execve(args[0], args, env);
		dup2(std_out_cpy, STDOUT_FILENO);
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
