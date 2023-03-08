#include "Cgi.hpp"
#include <sys/poll.h>

void	exiterr(std::string e)
{
	perror(e.c_str());
	exit(0);
}

char**	make_exec_arg(std::string program, std::string path_info)
{
	std::string	full_prog_path = path_info + program;
	char**	ret = new char*[3];

	ret[0] = strdup("/usr/local/bin/python3.9");
	ret[1] = strdup(full_prog_path.c_str());
	ret[2] = NULL;
	return (ret);
}

std::string	cgi(std::string program, std::string path_info)
{
	int			pid;
	char		buf[CGI_READ_SIZE];
	int			readret;
	std::string	ret;
	int			fds[2];
	int			status;

	if (pipe(fds) == -1)
		exiterr("pipe failed in cgi");
	if ((pid = fork()) == -1)
		exiterr("fork failed in cgi");
	if (pid == 0)
	{
		char**	args = make_exec_arg(program, path_info);

		std::cout << "args[1]: " << args[1] << std::endl;
		args[0] = args[1];
		args[1] = NULL;
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		execve(args[0], args, environ);
		exiterr("execve failed in cgi");
	}
	// waitpid(-1, NULL, 0);
	wait(&status);
	// fcntl(fds[0], F_SETFL, O_NONBLOCK);
	close(fds[1]);
	struct pollfd poll_fd = {fds[0], POLLIN, 0};
	while (1)
	{
		poll(&poll_fd, sizeof(struct pollfd), 0);
		readret = read(fds[0], buf, CGI_READ_SIZE);
		if (readret == -1)
			exiterr("read failed in cgi");
		if (readret == 0)
			break;
		ret += std::string(buf, readret);
	}
	close(fds[0]);
	return (ret);
}

int	main()
{
	std::cout << cgi("cgi_test.py", PATH_INFO);
}
