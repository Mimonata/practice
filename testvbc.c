/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   testvbc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 23:08:22 by spitul            #+#    #+#             */
/*   Updated: 2025/04/30 23:19:33 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

node *ret = low_prio(s);

node *low_prio(char **s)
{
	node *ret;
	node *right;

	ret = middle(s);
	while (accept(s, '+'))
	{
		right = middle(s);
		if (!right)
		{
			
		}
		node n = {.type = ADD, .l = ret, .r=right};
		ret = new_node(n);
	}
	return (ret);
}

node *middle(char **s)
{
	node *ret;
	node *right;

	ret = high;
	while (accept(s, '*'))
	{
		right = high(s);
		if (!right)
		{
			
		}
		node n = {.type = MULTI, .l = ret, .r = right};
		ret = new_node(n);
	}
	return (ret);
}

node *high(char **s)
{
	node *ret;

	if (accept(s, '('))
	{
		ret = low_prio(s);
		if (!expect(s, ')'))
		{
			destroy_tree(ret);
			return 0;
		}
		return (ret);
	}
	else 
		return (extract_num(s));
}

node *extract_num(char **s)
{
	if (isdigit(**s))
	{
		node n = {.type=VAL, .val = **s - '0', .l = 0, .r = 0};
		accept(s, **s);
		return (new_node(n));
	}
	unexpected(**s);
	return (0);
}