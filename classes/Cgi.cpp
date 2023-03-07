#include "Cgi.hpp"

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

std::string	cgi(std::string program, std::string path_info)
{
	int			pid;
	char		buf[CGI_READ_SIZE];
	int			readret;
	std::string	ret;
	int			status = 0;
	int			rand = std::rand();
	std::stringstream	ss;
	ss << rand;
	std::string	tmp_file = "_pipe_tmp_file" + ss.str();
	int	fdin = open(tmp_file.c_str(), O_CREAT | O_RDONLY, 777);
	if (fdin == -1)
		exiterr("open failed in cgi");
	int	fdout = open(tmp_file.c_str(), O_WRONLY);
	if (fdout == -1)
		exiterr("open failed in cgi");

	if ((pid = fork()) == -1)
		exiterr("fork failed in cgi");
	if (pid == 0)
	{
		char**	args = make_exec_arg(program, path_info);

		// printf("[DEBUG] args[0]: %s\n", args[0]);
		// printf("[DEBUG] args[1]: %s\n", args[1]);
		// fcntl(fd, F_SETFL, O_NONBLOCK);
		dup2(fdout, STDOUT_FILENO);
		execve(args[0], args, environ);
		exiterr("execve failed in cgi");
	}
	waitpid(-1, NULL, 0);
	sleep(10);
	while (1)
	{
		readret = read(fdin, buf, CGI_READ_SIZE);
		std::cout << "readret: " << readret << std::endl;
		if (readret == -1)
			exiterr("read failed in cgi");
		if (readret == 0)
			break;
		std::cout << "'" << ret << "'" << std::endl;
		ret += std::string(buf, readret);
	}
	close(fdin);
	close(fdout);
	remove(tmp_file.c_str());
	return (ret);
}

int	main()
{
	std::srand(std::time(NULL));
	std::cout << cgi("cgi_test.py", PATH_INFO);
}