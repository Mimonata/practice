/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picosh2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 11:56:35 by spitul            #+#    #+#             */
/*   Updated: 2025/05/09 12:40:33 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int picoshell(char **s[])
{
	pid_t	pid;
	int		i;
	int		pfd[2];
	int		prev;
	
	prev = STDIN_FILENO;
	i = 0;
	while (s[i])
	{
		if (s[i + 1])
		{				
			if (pipe(pfd) == -1)
				return -1;
		}
		pid = fork();
		if (pid == 0)
		{
			if (s[i + 1])
			{
				dup2(pfd[1], STDIN_FILENO)
			}
		}
	}
	
}
