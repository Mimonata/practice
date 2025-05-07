/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:41:40 by spitul            #+#    #+#             */
/*   Updated: 2025/05/07 08:18:00 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct json {
    enum { MAP, INTEGER, STRING } type;
    union {
        struct {
            struct pair *data;
            size_t size;
        } map;
        int integer;
        char *string;
    };
} json;

typedef struct pair {
    char *key;
    json value;
} pair;

int	ft_isspace(char c)
{
	if (c == 32 || (c >= 8 && c <= 13))
		return (1);
	return (0);
}

void	skip_ws(FILE *f)
{
	int	c;

	while (ft_isspace(c = getc(f)))
	ungetc(c, f);
}

int	pass_int(FILE *f, int *out)
{
	int	c;
	int	sign;
	int	val;

	val = 0;
	sign = 1;
	c = getc(f);
	if (c == '-')
	{
		sign = -1;
		c = getc(f);
	}
	if (!isdigit(c))
	{
		printf("Unexpected token %c \n", c);
		return (-1);
	}
	while(isdigit(c))
	{
		val = val *10 + (c - '0');
		c = getc(f);
	}
	//if (!isdigit(c))
		ungetc(c, f);
	*out = val * sign;
	return (1);
}

char	*parse_str(FILE *f)
{
	int	c;
	char	*buf;
	size_t	len;
	int		next;
	
	buf = malloc(1 * sizeof(char));
	len = 0;
	if (getc(f) != '"')
		return (NULL);
	while ((c = getc(f)) != EOF)
	{
		if (c == '"')
		{
			buf = realloc(buf, len + 1);
			buf[len] = '\0';
			return (buf);
		}
		else if (c == '\\')
		{
			next = getc(f);
			if (next == EOF)
				break;
			buf[len++] = next;
		}
		else
		{
			buf = realloc(buf, len + 1);
			buf[len++] = c;
		}
	}
	printf ("Unexpected end of input\n");
	free(buf);
	return (NULL);	
}

int	parse_map(json *dst, FILE *f)
{
	int	c;
	char	*key;
	json	val;

	dst->type = MAP;
	dst->map.data = NULL;
	dst->map.size = 0;
	key = NULL;
	if (getf(f) != '{')
	{
		printf("Unexpected token\n");
		return (-1);
	}
	skip_ws(f);
	c = getc(f);
	if (c == '}')
		return (1); //empty
	ungetc(f, c);
	while (1)
	{
		skip_ws(f);
		key = parse_str(f);
		if (!key)
			return (-1);
		skip_ws(f);
		if ((c = getc(f)) != ':')
		{
			printf("Unexpected token\n");
			free (key);
			return (-1);
		}
		skip_ws(f);
		if (argo(&val, f) == -1)
		{
			free (key);
			return (-1);
		}
		dst->map.data = realloc(dst->map.data, (dst->map.size + 1) * sizeof(pair));
		dst->map.data[dst->map.size].key = key;
		dst->map.data[dst->map.size].value = val;
		dst->map.size ++;
		skip_ws(f);
		c = getc(f);
		if (c == '}')
			break;
		if (c != ',')
		{
			printf("Unexpected token at %c", c); // why not free anything
			return (-1);
		}	
	}
	return (1);
}

int	argo(json *dst, FILE *stream)
{
	int	c;

	skip_ws(stream);
	c = getc(stream);
	if (c == EOF)
	{
		printf ("Unexpected end of fi\n");
		return (-1);
	}
	if (c == '"')
	{
		ungetc(c, stream);
		char *s;
		s = parse_str(stream);
		if (!s)
			return (-1);
		dst->type = STRING;
		dst->string = s;
		return(1);
	}
	else if (c == '-' || isdigit(c))
	{
		ungetc(c, stream);
		int	i;
		if (pass_int(stream, &i) == -1)
			return (-1);
		dst->type = INTEGER;
		dst->integer = i;
	}
	else if (c == '{')
	{
		ungetc(c, stream);
		return (parse_map(dst, stream));
	}
	else
	{
		printf("Unexpected token %c", c);
		return(-1);
	}
	return(1);
}


int	parse_map(json *dst, FILE *f)
{
	
}

int	argo(json *dst, FILE *stream)
{
	int c;

	skip_ws(stream);
	c = getc(stream);
	if (c == EOF)
	{
		printf("Unexpected EOF\n");
		return(-1);
	}
	if (c == '"')
	{
		ungetc(c, stream);
		char	s = parse_str(stream);
		if (!s)
			return(-1);
		dst->type = STRING;
		dst->string = s;
		return(1);
	}
	else if (c == '-' || isdigit(c))
	{
		ungetc(c, stream);
		int	i;
		if (pass_int(stream, &i) == -1)
			return (-1);
		dst->type = INTEGER;
		dst->integer = i;
	}
	else if (c == '{')
	{
		ungetc(c, stream);
		return(parse_map(dst, stream));
	}
	else
	{
		printf("Unexpected token %c\n", c);
		return(-1);
	}
}
