/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sand3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 12:22:22 by spitul            #+#    #+#             */
/*   Updated: 2025/05/02 12:56:19 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

pid_t	g_chil = -4;

void	handle_signal(int sig)
{
	(void)sig;
	if ()
}

int	sand(void (*f)(void), unsigned int timeout, bool verbose)
{
	struct sigaction	sa = {0};
	int					sig;
	int					status;

	g_chil = fork();
	if (g_chil < 0)
		return (-1);
	else if (g_chil == 0)
	{
		f();
		exit(0);
	}
	sa.sa_handler = handle_signal;
	if (sigaction(SIGALRM, &sa, NULL) == -1)
		return (-1);
	alarm(timeout);
	while (waitpid(g_chil, &status, NULL) == -1)
	{
		if (errno != EINTR)
			return (-1);
	}
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 0)
		{
			if (verbose)
				printf("Nice func!\n");
			return (1);
		}
		else
		{
			if (verbose)
				printf("Bad function %d\n", WEXITSTATUS(status));
			return (0);
		}
	}
	if (WIFSIGNALED(status))
	{
		sig =  WTERMSIG(status);
		if (sig == SIGKILL)
		{
			if (verbose)
			{
				printf ("Bad func timedout after %d\n", timeout);
				return (0);
			}
		}
		else 
		{
			if (verbose)
				printf("Bad function %s", strsignal(sig));
			return (0);
		}
	}
	return (-1);
}

ret = low()