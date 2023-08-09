#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

typedef enum pipe_flag_t pipe_flag_t;

enum pipe_flag_t
{
	READ,
	WRITE
};

void	ft_putstr_fd(int fd, const char *str)
{
	if (!str)
		return ;
	char *tmp = (char *)str;
	while (*tmp)
		tmp++;
	write(fd, str, tmp - str);
}

void	ft_error(const char *path, const char *str)
{
	ft_putstr_fd(STDERR_FILENO, "error: ");
	if (path)
		ft_putstr_fd(STDERR_FILENO, path);
	ft_putstr_fd(STDERR_FILENO, str);
}

void	ft_cd(const char *str)
{
	if (!str)
		ft_putstr_fd(STDERR_FILENO, "error: cd: bad arguments\n");
	if (chdir(str) < 0) {
		ft_putstr_fd(STDERR_FILENO, "error: cd: cannot change directory to ");
		ft_putstr_fd(STDERR_FILENO, str);
		ft_putstr_fd(STDERR_FILENO, "\n");
	}
}

int main(int ac, char **av, char **env)
{
	if (ac < 2)
		return (1);

	int fd[2] = {0, 0};
	int tmp = dup(STDIN_FILENO);
	int i = 1;

	while (av[i])
	{
		av = &av[i];

		i = 0;

		while (av[i] && strcmp(av[i], ";") && strcmp(av[i], "|"))
			i++;

		if (av[0] && !strcmp(av[0], "cd")) {
			if (i != 2) {
				ft_putstr_fd(STDERR_FILENO, "error: cd: bad arguments\n");
				continue ;
			}
			ft_cd(av[1]);
			continue ;
		}

		if (av[i] && !strcmp(av[i], "|")) {
			if (pipe(fd) < 0) {
				ft_putstr_fd(STDERR_FILENO, "error: fatal\n");
				break ;
			}

			av[i] = NULL;

			if (av[0] && !fork()) {
				close(fd[READ]); // on a pas besoin de lire, il n'y a pas de data
				dup2(fd[WRITE], STDOUT_FILENO);
				close(fd[WRITE]); // pour que pipe work on dois la fermer
				dup2(tmp, STDIN_FILENO);
				close(tmp);
				execve(av[0], av, env);
				ft_putstr_fd(STDERR_FILENO, "error: cannot execute ");
				ft_putstr_fd(STDERR_FILENO, av[0]);
				ft_putstr_fd(STDERR_FILENO, "\n");
				exit(1);
			}

			close(fd[WRITE]); // on ecrit pas
			close(tmp);
			tmp = fd[READ];
			i++;
			continue ;
		}

		if ((!av[i] || !strcmp(av[i], ";")))
		{
			if (av[i] && !strcmp(av[i], ";")) {
				av[i] = NULL;
				i++;
			}

			if (av[0] && !fork()) {
				dup2(tmp, STDIN_FILENO);
				close(tmp);
				execve(av[0], av, env);
				ft_putstr_fd(STDERR_FILENO, "error: cannot execute ");
				ft_putstr_fd(STDERR_FILENO, av[0]);
				ft_putstr_fd(STDERR_FILENO, "\n");
				exit(1);
			}
			close(tmp);
			while (waitpid(-1, NULL, WUNTRACED) != -1);
			tmp = dup(STDIN_FILENO);
		}
	}
	close(tmp);
	return (0);
}
