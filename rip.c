/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rip.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:16:38 by spitul            #+#    #+#             */
/*   Updated: 2025/03/05 18:25:41 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

bool	checker(char *s, int len)
{
	int	i;
	int	bal;

	i = 0;
	bal = 0;
	while (s[i])
	{
		if (s[i] == '(')
			bal ++;
		else if (s[i] == ')')
			bal --;
		if (bal < 0)
			return (0);
	}
	if (bal == 0)
		return (1);
	return (0);
}

void	rip(char *s, int len, int index)
{
	if (index == len)
	{
		if (checker(s, len))
			printf("%s", s);
	}
	rip(s, len, index + 1);
	s[index] = ' ';
	rip(s, len, index + 1);
}

int	main(int ac, char **av)
{
	int	len;

	if (ac != 2)
		return (-1);
	len = strlen(av[1]);
	rip(av[1], len, 0);
}
