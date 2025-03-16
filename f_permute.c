/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_permute.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:14:11 by spitul            #+#    #+#             */
/*   Updated: 2025/03/16 19:00:36 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

void	swap(char *s, int i, int j)
{
	char	t;

	t = s[i];
	s[i] = s[j];
	s[j] = t;
}

void	ft_sort(char *s)
{
	int	i;
	int	j;

	i = 0;
	while (s[i])
	{
		j = 0;
		while (s[j] && s[j])
		{
			if (s[j] > s[j + 1] && s[j + 1])
				swap(s, j, j + 1);
			j++;
		}
		i++;
	}
}

void	f_perm(char *s, int len, int index)
{
	int		i;
	char	new[len + 1];
	int		j;

	if (index == len - 1)
	{
		puts(s);
		return ;
	}
	
	i = index;
	while (i < len)
	{
		swap(s, i, index);
		j = 0;
		while (s[j])
		{
			new[j] = s[j];
			j++;
		}
		new[j] = '\0';
		ft_sort(&new[index + 1]);
		f_perm(new, len, index + 1);
		swap(s, i, index);
		i++;
	}
}

int	main(int ac, char **av)
{
	int	len;

	if (ac != 2)
		return (1);
	len = strlen(av[1]);
	ft_sort(av[1]);
	f_perm(av[1], len, 0);
	return (0);
}
