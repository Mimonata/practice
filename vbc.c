#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct node
{
	enum
	{
		ADD,
		MULTI,
		VAL
	} type;
	int			val;
	struct node	*l;
	struct node	*r;
}				node;

node	*new_node(node n)
{
	node	*ret;

	ret = calloc(1, sizeof(node));
	if (!ret)
		return (NULL);
	*ret = n;
	return (ret);
}

void	unexpected(char c)
{
	if (c)
		printf("Unexpected token '%c'\n", c);
	else
		printf("Unexpected end of input");
}

int	accept(char *s, char c)
{
	if (*s == c)
	{
		(*s)++;
		return (1);
	}
	return (0);
}

int	expect(char *s, char c)
{
	if (accept(s, c))
		return (1);
	unexpected(*s);
	return (0);
}

int	find_index(char **s)
{
	char	*str;
	int		take;
	int		i;
	int		accumulator;
	char	c;
	int		len;

	len = ft_strlen(*s);
	str = *s;
	accumulator = 0;
	take = -1;
	if (strlen(s) == 1 && isdigit(s[0])) // leaf
		return (0);
	i = 0;
	while (str[i])
	{
		c = str[i];
		if (c == '(')
			accumulator++;
		else if (c == ')')
			accumulator--;
		if (str[i] == '+' && accumulator == 0)
			take = i;
	}
	if (take >= 0)
		return (take);
	i = 0;
	while (str[i])
	{
		c = str[i];
		if (c == '(')
			accumulator++;
		else if (c == ')')
			accumulator--;
		if (str[i] == '*' && accumulator == 0)
			take = i;
	}
	if (take < 0)
	{
		*s = *s + 1;
		str[len - 1] = 0;
		find_index(s);
	}
	return (take);
}

node	*parse_node(char *s)
{
	int		take;
	node	n;

	take = find_index(&s);
	if (s[take] == '+' || s[take] == '*')
	{
		n.type = ADD;
		if (s[take] == '*')
			n.type = MULTI;
		n.l = parse_node(s);
		n.r = parse_node(&s[take + 1]);
	}
	else if (s[take] >= '0' && s[take] <= '9')
	{
		n.type = VAL;
		n.val = s[take] - '0';
	}
	else	
		write(2, "smthg wrong\n", 12);

	return(new_node(n));
}

void unexpected_exit(char c)
{
	unexpected(c);
	exit(1);
}

node	*parse_expr(char *s)
{
	node	*ret;

	validate(s);
	ret = parse_node(s);
	// while (accept(s, '+')) {
	//     node *right = parse_term(s);
	//     if (!right) {
	//         destroy_tree(left);
	//         return (NULL);
	//     }
	//     node n = { .type = ADD, .l = left, .r = right };
	//     left = new_node(n);
	// }
	return (ret);
}

int	eval_tree(node *tree)
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

int	main(int ac, char **av)
{
	if (ac != 2)
		return (1);
	node *tree = pars_expr(av[1]);
	if (!tree)
		return (1);
	printf("%d\n", eval_tree(tree));
}