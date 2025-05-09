#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int	size_mat(char **cmds[])
{
	int	i;

	i = 0;
	while (cmds[i] != NULL)
		i++;
	return (i);
}

/*int	picoshell(char **cmds[])
{
	pid_t	pid;
	int		pfd[2];
	int		last_in;
	int		i;
	int		last_in;
	int		pfd[2];
	int		i;
	pid_t	pid;

	last_in = STDIN_FILENO;
	i = 0;
	while(cmds[i])
	{
		if (cmds[i + 1])
		{
			if (pipe(pfd) == -1)
				return(perror("pipes"), 1);
		}
		pid = fork();
		if (pid == -1)
			return (perror("fork"), 1);
		if (pid == 0)
		{
			if (last_in != STDIN_FILENO)
			{
				dup2(last_in, STDIN_FILENO);
				close(last_in);
			}
			if (cmds[i + 1])
			{
				close(pfd[0]);
				dup2(pfd[1], STDOUT_FILENO);
				close (pfd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit (1);
		}
		else
		{
			if (last_in != STDIN_FILENO)
				close(last_in);
			if (cmds[i + 1])
			{
				last_in = pfd[0];
				close(pfd[1]);
			}
		}
		i++;
	}
	while (wait(NULL) > 0)
				;
	return (0);
}*/
int	picoshell(char **cmds[])
{
	last_in = STDIN_FILENO;
	i = 0;
	while (cmds[i])
	{
		if (cmds[i + 1])
		{
			if (pipe(pfd) == -1)
				return (perror("pipes"), 1);
		}
		pid = fork();
		if (pid == -1)
			return (perror("fork"), 1);
		if (pid == 0)
		{
			if (last_in != STDIN_FILENO)
			{
				dup2(last_in, STDIN_FILENO);
				close(last_in);
			}
			if (cmds[i + 1])
			{
				close(pfd[0]);
				dup2(pfd[1], STDOUT_FILENO);
				close(pfd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		else
		{
			if (last_in != STDIN_FILENO)
			{
				close(last_in);
			}
			if (cmds[i + 1])
			{
				last_in = pfd[0];
				close(pfd[1]);
			}
		}
		i++;
	}
	while (wait(NULL) > 0)
		;
	return (0);
}

int	main(int argc, char **argv)
{
	int		cmds_size;
	char	***cmds;
	int		cmds_i;
	int		ret;

	cmds_size = 1;
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "|"))
			cmds_size++;
	}
	cmds = calloc(cmds_size + 1, sizeof(char **));
	if (!cmds)
	{
		dprintf(2, "Malloc error: %m\n");
		return (1);
	}
	cmds[0] = argv + 1;
	cmds_i = 1;
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "|"))
		{
			cmds[cmds_i] = argv + i + 1;
			argv[i] = NULL;
			cmds_i++;
		}
	}
	ret = picoshell(cmds);
	if (ret)
		perror("picoshell");
	free(cmds);
	return (ret);
}
