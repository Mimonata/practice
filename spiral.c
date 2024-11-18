/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spiral.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:40:48 by spitul            #+#    #+#             */
/*   Updated: 2024/11/17 20:18:40 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

#define N 31

void	set_half_line(int matrix[N][N], int x, int y, int n)
{
	while (x < n)
	{
		matrix[x][y] = 3;
		x ++;
	}
}
void	set_half_column(int matrix[N][N], int x, int y, int n)
{
	while (y <= n)
	{
		if ((y - n != 1) && (y - x != 1))
			matrix[x][y] = 3;
		y ++;
	}
}

int spiral_check(int matrix[N][N], int m)
{
	int	x;
	int	y;

	x = 0;
	y = 2;

	while (y < N)
	{
		x = 0;
		while (x < N)
		{
			if ((y % 2 == 0) && (y < N / 2) && (x == y - 2))
			{
				matrix[x][y] = 2; 
				set_half_line(matrix, x, y, N / 2);
				set_half_column(matrix, x, y, N / 2);
			}
			if (((y % 2 == 1) && (N % 2 == 0) || (y % 2 == 0) && (N % 2 == 1)) && (y > N / 2) && (x == y))
			{
				matrix[x][y] = 4;
				set_half_line(matrix, N / 2, y, x);
				set_half_column(matrix, x, N / 2, y);
			}
			if ((y % 2 == 0) && (y < N / 2) && (y == N - x - 1))
			{
				matrix[x][y] = 5; 
				set_half_line(matrix, N / 2, y, x);
				set_half_column(matrix, x, y, N / 2);
			}
			if (((y % 2 == 1) && (N % 2 == 0) || (y % 2 == 0) && (N % 2 == 1)) && (y > N / 2) && (y == N - x - 1))
			{
				matrix[x][y] = 6;
				set_half_line(matrix, x, y, N / 2);
				set_half_column(matrix, x, N / 2, y);
			}
			
			x ++;
		}
		y ++;
	}
}

void	spiralize(int matrix[N][N])
{
	int	x;
	int	y;
	int	m;

	x = 0;
	y = 0;
	while (y < N)
	{
		x = 0;
		while (x < N)
		{
			if ((y == 0) || (x == N - 1) || y == N - 1 || (x == 0 && y != 1))
				matrix[x][y] = 1;
			else 
				matrix[x][y] = 0;
			x ++;
		}
		y ++;
	}
	if (N % 2 == 0)
	{
		m = N / 2;
		spiral_check(matrix, m);
	}
	else
	{
		m = N / 2 + 1;
		spiral_check(matrix, m);
	}
}

int	main(void)
{
	int	matrix[N][N];
	int	x;
	int	y;

	x = 0;
	y = 0;
	spiralize(matrix);
	while (y < N)
	{
		x = 0;
		while (x < N)
		{
			if (matrix[x][y] != 0)
			printf("\033[38;2;255;0;255m");
			printf("%d ", matrix[x][y]);
			printf("\033[38;2;34;139;34m");
			x ++;
		}
		printf("\n");
		y ++;
	}
	return (0);	
}
