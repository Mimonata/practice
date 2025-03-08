/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   powerset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 07:16:06 by spitul            #+#    #+#             */
/*   Updated: 2025/02/21 22:11:58 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void	print(int *set, int n)
{
	int i;
if (n == 0)  // Do not print if the subset is empty
        return;
	i= 0;
	while (i<n)
	{
		printf("%d", set[i]);
		i ++;
	}
	printf("\n");
}

int	sum_set(int *set, int len)
{
	int i = 0;
	int sum = 0;

	while (i < len)
	{
		sum += set[i];
		i ++;
	}
	return (sum);
}


void	powerset(int *set, int *subset, int n, int sum, int index, int lenset)
{
// 	printf("Checking subset: ");
// for (int i = 0; i < lenset; i++)
//     printf("%d ", subset[i]);
// printf("\n");

	
	if (index == n)
	{
		if (sum_set(subset, lenset) == sum)
			print(subset, lenset);
		return ;
	}
	powerset(set, subset, n, sum, index + 1, lenset);
	subset[lenset] = set[index];
	powerset(set, subset, n, sum, index + 1, lenset + 1);
}

int	main(int ac, char **av)
{
	int	sum, i, j;
	int *set;
	int *subset;

	i = 2;
	j = 0;
	printf("%d\n", ac);
	sum = atoi(av[1]);
	printf("%d\n", sum);
	if (ac <= 2)
		return (1);
	subset = malloc((ac - 2) * sizeof(int));
	set = malloc((ac-2) * sizeof(int));
	while (i < ac)
	{
		set[j] = atoi(av[i]);
		//printf("%d ", set[j]);
		i ++;
		j ++;
	}
	//write(1, "\n", 1);
	powerset(set, subset, ac-2, sum, 0, 1);
	free (set);
	free (subset);
	return 0;
}
