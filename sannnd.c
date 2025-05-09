/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sannnd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 09:00:10 by spitul            #+#    #+#             */
/*   Updated: 2025/05/09 11:36:01 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>
#include <errno.h>

pid_t	g_chil = -4;

void	handle_sig(int sig)
{
	(void)sig;
	if (g_chil > 0)
		kill(g_chil, SIGKILL);
}

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	struct sigaction	sa = {0};
	int	e_c;
	int	status;

	e_c = 0;
	g_chil = fork();
	if (g_chil == -1)
		return -1;
	if (g_chil == 0)
	{
		f();
		exit (0);
	}
	sa.sa_handler = handle_sig;
	if (sigaction(SIGALRM, &sa, NULL) == -1)
		return -1;
	alarm(timeout);

	while (waitpid(g_chil, &status, NULL) == -1)
	{
		if (errno != EINTR)
			return -1;
	}
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 0)
		{
			if (verbose)
				printf("NIce");
			return 1;
		}
		else
		{
			if (verbose)
				printf("BAd with exit code %d", WEXITSTATUS(status));
			return 0;
		}
	}
	if (WIFSIGNALED(status))
	{
		e_c = WTERMSIG(status);
		if (verbose)
		{
			if (e_c == SIGKILL)
				printf("Function timedout");
			else
				printf("Function term by signal %s", strsignal(e_c));
		}
		return 0;
	}
	return -1;
}
