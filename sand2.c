/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sand2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 12:47:31 by spitul            #+#    #+#             */
/*   Updated: 2025/04/26 09:56:01 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

pid_t	g_child_pid = -4;

void	handle_timeout(int sig)
{
	(void)sig;
	if (g_child_pid > 0)
		kill(g_child_pid, SIGKILL);
}

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	int		status;
	int		e;
	struct sigaction	sa = {0};
	
	g_child_pid = fork();
	if (g_child_pid < 0)
		return (-1);
	if (g_child_pid == 0)
	{
		f();
		exit (0);
	}
	sa.sa_handler = handle_timeout;
	if (sigaction(SIGALRM, &sa, NULL) == -1)
		return (-1);
	alarm(timeout);
	while (waitpid(g_child_pid, &status, 0) == -1)
	{
		if (errno != EINTR)
			return (-1);
	}
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
				printf ("Bad function: %s\n", strsignal(e));
		}
		return (0);
	}
	return (-1);
}

void	goodf(void)
{
	return ;
}

void	timeo(void)
{
	sleep(10);
}

void	segf(void)
{
	int *p;
	
	p = NULL;
	*p = 1;
}

int main()
{
	sandbox(timeo, 3, true);
	sandbox(segf, 3, true);
	printf("goodf %d \n", sandbox(goodf, 3, false));
	
	return (0);
}
