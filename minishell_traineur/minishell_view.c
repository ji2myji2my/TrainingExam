#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

static void    perr(char *str, char *arg)
{
    while (*str)
        write(2, str++, 1);
    if (arg)
        while(*arg)
            write(2, arg++, 1);
    write(2, "\n", 1);
}

static int    exec(char **argv, char **envp, int i, int fd[2], int tmp_fd)
{
    if (argv[i] && !strcmp(argv[i], "|"))
        pipe(fd);
    if (!fork())
    {
        if (argv[i] && !strcmp(argv[i], "|"))
        {
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
        }
        argv[i] = NULL;
        dup2(tmp_fd, 0);
        close(tmp_fd);
        execve(argv[0], argv, envp);
        return (perr("error: cannot execute ", argv[0]), 1);
    }
    waitpid(-1, NULL, 0);
    return (0);
}

int    main(int argc, char **argv, char **envp)
{
    (void)argc;
    int    i;
    int    fd[2];
    int    tmp_fd = dup(0);

    while (argv[0] && argv[1])
    {
        i = 0;
        argv++;
        while (argv[i] && strcmp(argv[i], ";") && strcmp(argv[i], "|"))
            i++;
        if (argv[i] && !strcmp(argv[i], "cd"))
        {
            if (i != 2)
                perr("error: cd: bad arguments", NULL);
            else if (chdir(argv[1]))
                perr("error: cd: cannot change directory to ", argv[1]);
        }
        else if (i)
            exec(argv, envp, i, fd, tmp_fd);
        if (argv[i] && !strcmp(argv[i], "|"))
        {
            close(fd[1]);
            close(tmp_fd);
            tmp_fd = fd[0];
        }
        argv += i;
    }
    close(tmp_fd);
    return (0);
}
