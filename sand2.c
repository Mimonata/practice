/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sand.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 12:47:31 by spitul            #+#    #+#             */
/*   Updated: 2025/04/19 16:01:32 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>

pid_t	g_child = -1;

void	handle_timeout(int sig)
{
	(void)sig;
	if (g_child > 0)
		kill(g_child, SIGKILL);
}

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t	pid;
	int		status;
	int		e;
	struct sigaction	sa = {0};
	
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		f();
		exit (0);
	}
	sa.sa_handler = handle_timeout;
	if (sigaction(SIGALRM, &sa, NULL) == -1)
		return (-1);
	g_child = pid;
	alarm(timeout);
	waitpid(pid, &status, NULL);
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 0)
		{
			if (verbose)
				printf("Nice function!\n");
			return (1);
		}
		else 
		{
			if (verbose)
				printf("Bad function exited with code %d\n", WEXITSTATUS(status));
			return (0);
		}
	}
	if (WIFSIGNALED(status))
	{
		e = WTERMSIG(status);
		if (verbose)
		{
			if (e == SIGKILL)
				printf("Bad function: function timed out after %d seconds\n", timeout);
			else
				printf ("Bad function: %d\n", strsignal(e));
		}
		return (0);
	}
	return (-1);
}



int main()
{
	
}
