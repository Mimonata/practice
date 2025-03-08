/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rip.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:16:38 by spitul            #+#    #+#             */
/*   Updated: 2025/03/07 12:30:16 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int	counter(char *s, int len)
{
	int	i;
	int	op;
	int	cl;

	if (!checker(s, len))
		return (0);
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
	int	bal;

	i = 0;
	bal = 0;
	while (s[i])
	{
		if (s[i] == '(')
			bal++;
		else if (s[i] == ')')
			bal--;
		if (bal < 0)
			return (0);
		i ++;
	}
	if (bal == 0)
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
	if (index == len)
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
