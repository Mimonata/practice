/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vbc_e.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:15:35 by spitul            #+#    #+#             */
/*   Updated: 2025/05/21 17:59:54 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;

int	g_error = -1;

node	*low(char **s);

node *new_node(node n)
{
    node *ret = calloc(1, sizeof(node));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

void unexpected_char(char c)
{
    if (c)
        printf("Unexpexted token '%c'\n", c);
    else
        printf("Unexpexted end of input \n");
}

int accept(char **s, char c)
{
    if (**s == c)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s,c))
        return (1);
    unexpected_char(**s);
    return (0);
}

node	*extract_num(char **s)
{
	int	val;

	val = 0;
	if (isdigit(**s))
	{
		val = **s - '0';
		node n = {.type =  VAL, .val = val, .l = NULL, .r = NULL};
		accept(s, **s);
		return new_node(n);
	}
	//unexpected_char(**s);
	g_error = 1;
	return NULL;
}

node	*high(char **s)
{
	node *ret;

	if (accept(s, '('))
	{
		ret = low(s);
		if (!accept(s, ')'))
		{
			g_error = 1;
			destroy_tree(ret);
			return NULL;
		}
		return ret;
	}
	return extract_num(s);
}

node	*middle(char **s)
{
	node	*ret;
	
	ret = high(s);
	while(accept(s, '*'))
	{
		node *right;
		right = high(s);
		if (!right)
		{
			destroy_tree(ret);
			return NULL;
		}
		node n = {.type = MULTI, .l = ret, .r = right};
		ret = new_node(n);
	}
	return ret;
}

node	*low(char **s)
{
	node	*ret;
	
	ret = middle(s);
	while(accept(s, '+'))
	{
		node *right;
		right = middle(s);
		if (!right)
		{
			destroy_tree(ret);
			return NULL;
		}
		node n = {.type = ADD, .l = ret, .r = right};
		ret = new_node(n);
	}
	return ret;
}

node *parse_expr(char **s)
{
   node	*ret;

   ret = low(s);
    if(**s || (g_error == 1))
    {
		unexpected_char(**s);
        destroy_tree(ret);
        return (NULL);
    }
    return (ret);
}

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
}

int main (int argc, char** argv)
{
    if (argc!= 2)
        return 1;
    node *tree = parse_expr(&argv[1]);
    if (!tree)
        return 1;
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
    return (0);
}