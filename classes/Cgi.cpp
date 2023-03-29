#include "Cgi.hpp"

void	Cgi::exiterr(std::string e)
{
	perror(e.c_str());
	exit(0);
}

char**	Cgi::_make_exec_arg(std::string program, std::string path_info)
{
	std::string	full_prog_path = path_info + program;
	char**	ret = new char*[2];

	ret[0] = strdup(full_prog_path.c_str());
	if (ret[0] == NULL)
		throw(CgiSystemFailure());
	ret[1] = NULL;
	return (ret);
}

std::string	Cgi::cgi(std::string program, std::string path_info)
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
		char**	args = _make_exec_arg(program, path_info);

		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		execve(args[0], args, environ);
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

bool	is_cgiable(std::string location, ConfigServer server)
{
	std::vector<Location>::iterator	it = server.locations.begin();

	for (; it != server.locations.end(); it++)
	{
		if ((*it).location == location)
		{
			if ((*it).cgi == true)
				return (true);
			else
				return (false);
		}
	}
	return (false);
}
