/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rip.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:16:38 by spitul            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/03/09 21:04:35 by spitul           ###   ########.fr       */
=======
/*   Updated: 2025/03/16 19:01:43 by spitul           ###   ########.fr       */
>>>>>>> cd72b8b8d52a2ab2922743845a59a260f9204a6d
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

bool	checker(char *s, int len);

int	counter(char *s, int len)
{
	int	i;
	int	op;
	int	cl;

	op = 0;
	cl = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '(')
			op++;
		else if (s[i] == ')')
			cl++;
		i ++;
	}
	return (op - cl);
}

bool	checker(char *s, int len)
{
	int	i;
	int	balance;

	i = 0;
	balance = 0;
	while (s[i])
	{
		if (s[i] == '(')
			balance++;
		else if (s[i] == ')')
			balance--;
		if (balance < 0)
			return (0);
		i ++;
	}
	if (balance == 0)
		return (1);
	return (0);
}

bool	checker_2(char *s, int len, int rem)
{
	int	i;
	int	repl;
	int	max;

	i = 0;
	repl = 0;
	max = counter(s, len);
	while (s[i])
	{
		if (s[i] == ' ')
			repl++;
		i++;
	}
	if (repl != max || rem != max)
		return (0);
	return (1);
}

void	rip(char *s, int len, int index, int rem)
{
	if (index == (len - 1))
	{
		if (checker_2(s, len, rem))
			printf("%s", s);
	}
	rip(s, len, index + 1, rem);
	s[index] = ' ';
	rip(s, len, index + 1, rem);
}

int	main(int ac, char **av)
{
	int	len;
	int max;

	if (ac != 2)
		return (-1);
	len = strlen(av[1]);
	max = counter(av[1], len);
	rip(av[1], len, 0, max);
}
