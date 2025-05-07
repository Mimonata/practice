/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picosh.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 07:31:43 by spitul            #+#    #+#             */
/*   Updated: 2025/05/03 08:49:35 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int	picoshell(char **cmds[])
{
	int	prev_fd;
	int	i;
	int	pfd[2];
	pid_t	pid;

	prev_fd = STDIN_FILENO;
	i = 0;
	while (cmds[i])
	{
		if (cmds[i + 1])
		{
			if (pipe(pfd) < 0)
				return (1);
		}
		pid = fork();
		if (pid < 0)
			return (1);
		if (pid == 0)
		{
			if (prev_fd != STDIN_FILENO)
			{
				dup2(prev_fd, STDIN_FILENO);
				close (prev_fd);
			}
			if (cmds[i + 1])
			{
				dup2(pfd[1], STDOUT_FILENO);
				close(pfd[1]);
				close(pfd[0]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit (1);
		}
		else
		{
			if (prev_fd != STDIN_FILENO)
				close(prev_fd);
			if (cmds[i + 1])
			{
				prev_fd = pfd[0];
				close (pfd[1]);
			}
		}
		i ++;
	}
	while (wait(NULL) < 0)
		;
	return (0);
}

int	main(int argc, char **argv)
{
	int	cmds_size = 1;
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "|"))
			cmds_size++;
	}
	char ***cmds = calloc(cmds_size + 1, sizeof(char **));
	if (!cmds)
	{
		dprintf(2, "Malloc error: %m\n");
		return 1;
	}
	cmds[0] = argv + 1;
	int cmds_i = 1;
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "|"))
		{
			cmds[cmds_i] = argv + i + 1;
			argv[i] = NULL;
			cmds_i++;
		}
	}
	int ret = picoshell(cmds);
	if (ret)
		perror("picoshell");
	free(cmds);
	return ret;
}
