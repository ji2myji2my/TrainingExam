#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void	my_stderr(char *str, char *av)
{
	while(*str)
		write(2, str++, 1);
	if (av)
		while(*av)
			write(2, av++, 1);
	write(2, "\n", 1);
}

int	my_exec(char **av, int idx, int tmp_fd, int fd[2], char **env)
{
	if (av[idx] && !strcmp(av[idx], "|"))
		pipe(fd);
	if (!fork())
	{
		if (av[idx] && !strcmp(av[idx], "|"))
		{
			dup2(fd[1], 1);
			close(fd[0]); // TODO MISS
			close(fd[1]);
		}
		av[idx] = NULL; // TODO MISS
		dup2(tmp_fd, 0); // TODO aproxi
		close(tmp_fd); // TODO MISS
		execve(av[0], av, env); // TODO approxi
		return (my_stderr("error: cannot execute ", av[0]), 1); // TODO MISS
	}
	waitpid(-1, NULL, 0); // TODO MISS
	return (0);
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	int	idx;
	int	fd[2];
	int	tmp_fd = dup(0);

	while (av[0] && av[1])
	{
		idx = 0;
		while (av[idx] && strcmp(av[idx], ";") && strcmp(av[idx], "|"))
			idx++;
		if (av[idx] && !strcmp(av[idx], "cd"))
		{
			if (idx != 2)
				my_stderr("error: cd: bad arguments", NULL);
			else if (chdir(av[1]))
				my_stderr("error: cd: cannot change directory to", av[1]);
		}
		else if (idx)
			my_exec(av, idx, tmp_fd, fd, env);
		if (av[idx] && !strcmp(av[idx], "|"))
		{
			close(fd[1]); // TODO MISS
			close(fd[0]); // TODO MISS
			tmp_fd = fd[0]; // TODO MISS
		}
		av += idx;
	}
	close(tmp_fd);
	return (0);
}
