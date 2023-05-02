#include "Cgi.hpp"
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/poll.h>

void	Cgi::exiterr(std::string e)
{
	perror(e.c_str());
	exit(0);
}

char**	Cgi::_make_exec_arg(std::string program, std::string path_info, std::string body)
{
	std::string	full_prog_path = path_info + program;
	char**	ret = new char*[3];

	ret[0] = strdup(full_prog_path.c_str()); //THis is a malloc right? So it can fail? do we need to free it?  What happens if you just c_str() it?(Tiemen)
	if (ret[0] == NULL)
		throw(CgiSystemFailure());
	ret[1] = strdup(body.c_str());
	if (ret[1] == NULL)
		throw(CgiSystemFailure());
	ret[2] = NULL;
	return (ret);
}

std::string	Cgi::cgi(std::string program, std::string path_info, std::string body)
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
		char**	args = _make_exec_arg(program, path_info, body);

		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		//write(fds[1], &body, size);
		execve(args[0], args, environ);
		throw(CgiSystemFailure());
	}

	wait(&status);
	close(fds[1]);
	//Can you please explain this? Why do you use poll, what is the purpose of this code(Tiemen)
	struct pollfd poll_fd = {fds[0], POLLIN, 0};
	while (1)
	{
		poll(&poll_fd, 1, 0);
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
