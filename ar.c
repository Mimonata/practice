/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ar.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 20:53:21 by spitul            #+#    #+#             */
/*   Updated: 2025/05/08 22:45:06 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

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

int	argo(json *dst, FILE *stream);

void	skip_ws(FILE *f)
{
	int	c;
	
	c = getc(f);
	while (c == 32 || (c >= 8 && c <= 13))
		c = getc(f);
	ungetc(c, f);
}

int	pass_int(FILE *f, int *out)
{
	int	c;
	int	si;
	int	val;

	val = 0;
	si = 1;
	c = getc(f);
	if (c == '-')
	{
		c = getc(f);
		si = -1;
	}
	if (!isdigit(c))
	{
		printf("Unexpected t %c", c);
		return -1;
	}
	while(isdigit(c))
	{
		val = val * 10 + (c - '0');
		c = getc(f);
	}
	ungetc(c, f);
	*out = val * si;
	return 1;
}

char	*parse_str(FILE *f)
{
	char	*out;
	int		c;
	int		len;
	
	c = getc(f);
	if (c != '"')
		return NULL;
	c = getc(f);
	len = 0;
	out = malloc(1);
	while (c != EOF)
	{
		if (c == '"')
		{
			out = realloc(out, len + 1);
			out[len] = '\0';
			return (out);
		}
		else if (c == '\\')
		{
			int	next;
			next = getc(f);
			out = realloc(out, len + 1);
			out[len++] = next;
		}
		else
		{
			out = realloc(out, len + 1);
			out[len++] = c;
		}
		c = getc(f);
	}
	printf("Unexpected end of input\n");
	free(out);
	return NULL;
}

int	parse_map(json *dst, FILE *f)
{
	int	c;
	char *key;
	json val;
	
	c = getc(f);
	if (c != '{')
	{
		printf("Unexpected to %c", c);
		return(-1);
	}
	skip_ws(f);
	c = getc(f);
	if (c == '}')
		return 1;
	ungetc(c, f);
	dst->type = MAP;
	dst->map.data = NULL;
	dst->map.size = 0;
	while (1)
	{
		skip_ws(f);
		key = parse_str(f);
		if (!key)
			return -1;
		skip_ws(f);
		c = getc(f);
		if (c != ':')
		{
			printf("Unexpect %c\n", c);
			free (key);
			return -1;
		}
		skip_ws(f);
		if (argo(&val, f) == -1)
		{
			free(key);
			return -1;
		}
		dst->map.data = realloc(dst->map.data, (dst->map.size + 1) * sizeof(pair));
		dst->map.data[dst->map.size].key = key;
		dst->map.data[dst->map.size].value = val;
		dst->map.size ++;
		skip_ws(f);
		if ((c = getc(f)) == '}')
			break;
		if (c != ',')
		{
			printf("Unexpected %c\n", c);
			free(key);
			return -1;
		}
	}
	return 1;
}

int	argo(json *dst, FILE *stream)
{
	int	c;
	
	skip_ws(stream);
	c = getc(stream);
	if (c == EOF)
	{
		printf("Unexpected end of input\n");
		return -1;
	}
	if (c == '{')
	{
		ungetc(c, stream);
		return parse_map(dst, stream);
	}
	else if (c == '"')
	{
		ungetc(c, stream);
		char	*s = parse_str(stream);
		if (!s)
			return -1;
		dst->type = STRING;
		dst->string = s;
		return 1;
	}
	else if (c == '-' || isdigit(c))
	{
		ungetc(c, stream);
		int	i;
		if (pass_int(stream, &i) == -1)
			return -1;
		dst->type = INTEGER;
		dst->integer = i;
		return 1;
	}
	else 
	{
		printf("Unexpected %c\n", c);
		return -1;
	}
}

void print_json(json *j);

void print_map(json *j) {
    printf("{");
    for (size_t i = 0; i < j->map.size; i++) {
        if (i > 0)
            printf(", ");
        printf("\"%s\": ", j->map.data[i].key);
        print_json(&j->map.data[i].value);
    }
    printf("}");
}

void print_json(json *j) {
    if (j->type == MAP) {
        print_map(j);
    } else if (j->type == INTEGER) {
        printf("%d", j->integer);
    } else if (j->type == STRING) {
        printf("\"%s\"", j->string);
    }
}

// Optional: a cleanup function to free allocated memory.
void free_json(json *j) {
    if (j->type == MAP) {
        for (size_t i = 0; i < j->map.size; i++) {
            free(j->map.data[i].key);
            free_json(&j->map.data[i].value);
        }
        free(j->map.data);
    } else if (j->type == STRING) {
        free(j->string);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <inputfile>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("fopen");
        return 1;
    }

    json root;
    if (argo(&root, f) == 1) {
        print_json(&root);
        printf("\n");
        free_json(&root);
    } else {
        printf("Failed to parse JSON.\n");
    }

    fclose(f);
    return 0;
}
