/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbc_e.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:14:34 by spitul            #+#    #+#             */
/*   Updated: 2025/05/21 17:14:36 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	evaluate_expression(char **expr, int *exit_code);
int	evaluate_term(char **expr, int *exit_code);
int	evaluate_factor(char **expr, int *exit_code);

int	evaluate_factor(char **expr, int *exit_code)
{
	int	result;

	result = 0;
	if (**expr == '(')
	{
		(*expr)++;
		if (**expr == '\0')
		{
			printf("Unexpected end of input\n");
			*exit_code = 1;
			return (0);
		}
		result = evaluate_expression(expr, exit_code);
		if (*exit_code)
			return (0);
		if (**expr == ')')
			(*expr)++;
		else
		{
			printf("Unexpected end of input\n");
			*exit_code = 1;
			return (0);
		}
	}
	else if (isdigit(**expr))
	{
		result = **expr - '0';
		(*expr)++;
		if(isdigit(**expr))
		{
		printf("unexpected token '%c'\n", **expr);
		*exit_code = 1;
		return (0);
		}
	}
	else
	{
		printf("unexpected token '%c'\n", **expr);
		*exit_code = 1;
		return (0);
	}
	return (result);
}

int	evaluate_term(char **expr, int *exit_code)
{
	int	result;

	result = evaluate_factor(expr, exit_code);
	while (**expr == '*')
	{
		(*expr)++;
		if (**expr == '\0')
		{
			printf("unexpected end of input\n");
			*exit_code = 1;
			return (0);
		}
		result *= evaluate_factor(expr, exit_code);
	}
	return (result);
}

int	evaluate_expression(char **expr, int *exit_code)
{
	int	result;

	result = 0;
	result = evaluate_term(expr, exit_code);
	while (**expr == '+')
	{
		(*expr)++;
		if (**expr == '\0')
		{
			printf("unexpected end of input\n");
			*exit_code = 1;
			return (0);
		}
		result += evaluate_term(expr, exit_code);
	}
	return (result);
}

int	main(int argc, char **argv)
{
	int		result;
	int		exit_code;
	char	*expr;

	if (argc != 2)
		return (1);
	expr = argv[1];
	exit_code = 0;
	//this part is to deal with +1 syntax error
	// if ((*expr == ')' || !isdigit(*expr)) && *expr != '(')
	// {
	// 	printf("unexpected token '%c'\n", *expr);
	// 	return (1);
	// }
	result = evaluate_expression(&expr, &exit_code);
	if (exit_code)
		return (1);
	if (*expr != '\0')
	{
		printf("unexpected token '%c'\n", *expr);
		return (1);
	}
	printf("%d\n", result);
	return (0);
}