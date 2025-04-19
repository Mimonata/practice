/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pico.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:10:11 by spitul            #+#    #+#             */
/*   Updated: 2025/03/17 14:19:41 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int	picoshell(char **cmds[])
{
	int		fdpipe[2];
	pid_t	pid;

	
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
