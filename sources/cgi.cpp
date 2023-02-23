#include <iostream>
#include <unistd.h>

#define CGI_READ_SIZE 100

void	exiterr(std::string e)
{
	std::cerr << e << std::endl;
	perror("error: ");
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

std::string	cgi(std::string program, char **envp, std::string path_info)
{
	int			pid;
	int			pipefds[2];
	char		buf[CGI_READ_SIZE];
	std::string	ret;

	if (pipe(pipefds) == -1)
		exiterr("pipe failed in cgi");
	if ((pid = fork()) == -1)
		exiterr("fork failed in cgi");
	if (pid == 0)
	{
		char**	args = make_exec_arg(program, path_info);

		// printf("[DEBUG] args[0]: %s\n", args[0]);
		// printf("[DEBUG] args[1]: %s\n", args[1]);
		dup2(pipefds[1], STDOUT_FILENO);
		close(pipefds[0]);
		execve(args[0], args, envp);
		exiterr("execve failed in cgi");
	}
	waitpid(-1, NULL, 0);
	close(pipefds[1]);
	while (read(pipefds[0], buf, CGI_READ_SIZE))
	{
		ret += buf;
	}
	return (ret);
}
