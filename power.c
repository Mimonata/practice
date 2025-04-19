/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   power.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 06:29:22 by spitul            #+#    #+#             */
/*   Updated: 2025/03/10 07:09:01 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	powerset(int set[], int target, int len, int subs[], int sizesub, int currentSum, int index)
{
	int i = 0;
	
	if (currentSum == target)
	{
		while (i < sizesub)
		{
			printf("%d ", subs[i]);
			i ++;
		}
		printf("\n");
	}
	if (index >= len || currentSum > target)
		return ;
	subs[sizesub] = set[index];
	powerset(set, target, len, subs, sizesub + 1, currentSum + set[index], index + 1);
	powerset(set, target, len, subs, sizesub, currentSum, index + 1);
}

int main(void)
{
	//int n = atoi(av[1]);
	int len = 6;
	int set[] = {2, 3, 4, 5, 6, 7};
	int subs[6];
	// int i;

	// i = 0;
	// while (i < len)
	// {
	// 	set[i] = atoi(av[i + 2]);
	// 	i ++;
	// }
	powerset(set, 10, len, subs, 0, 0, 0);
	return 0;
}
