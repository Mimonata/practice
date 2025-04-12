/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   morse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:48:10 by spitul            #+#    #+#             */
/*   Updated: 2025/04/12 09:51:46 by spitul           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

const char *const morse[55] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.",
	"....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-",
	".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----",
	".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..",
	"----.", ".-.-.-", "--..--", "..--..", ".----.", "-.-.--", "-..-.", "-.--.",
	"-.--.-", ".-...", "---...", "-.-.-.", "-...-", ".-.-.", "-....-", "..--.-",
	".-..-.", "...-..-", ".--.-.", "...---..."};
const char *const ascii[55] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
	"K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y",
	"Z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", ",", "?", "'",
	"!", "/", "(", ")", "&", ":", ";", "=", "+", "-", "_", "\"", "$", "@",
	"SOS"};

static char	*skip_space(char *s)
{
	if (!s)
		return (NULL);
	// while (s[*index] && ((s[*index] >= 8 && s[*index] <= 13) || s[*index] == 32))
	// 	(*index)++;
	// return (&(s[*index]));
	// }
	// else
	// {
	while (*s && ((*s >= 8 && *s <= 13) || *s == 32))
		s ++;
	return (s);
}

static int	is_space(char c)
{
	if (c && ((c >= 8 && c <= 13) || c == 32))
		return (1);
	return (0);
}

static bool	str_has_char(char *s)
{
	bool	flag;

	if (!s)
		return (0);
	flag = 0;
	while (*s && !flag)
	{
		if (!is_space(*s))
			flag = 1;
		s ++;
	}
	return (flag);
}

static int	get_size(char *s)
{
	int	size;
	char	*token;
	int	*i;

	if (!s)
		return (0);
	s = skip_space(s);
	token = strstr(s, "   ");
	if (token == NULL && str_has_char(s))
		return (1);
	size = 1;
	s = skip_space(token); 
	while (token != NULL)
	{
		token = strstr(s, "   ");
		if (token != NULL)
		{
			s = skip_space(token);
			size ++;
		}
		else if (token == NULL && str_has_char(s))
			size ++;
	}
	return (size);
}

static int	match_char(char *s)
{
	int	i;
	int	k;
	int	found;

	if (!s)
		return (0);
	i = 0;
	k = 0;
	found = 0;
	while (morse[i] && !found && s[k])
	{
		k = 0;
		while (morse[i][k] && !found && s[k])
		{
			if (s[k] == morse[i][k] && s[k])
				k ++;
			else if (s[k] != morse[i][k] && s[k])
				break ;
			else if (s[k] == 0)
			{
				found = 1;
				printf("%c", ascii[i]);
				break ;
			}
		}
		i ++;
	}
	return (found);
}
static int	match_first(char *s)
{
	int	i;
	int	found;
	
	if (!s)
		return (0);
	i = 0;
	found = 0;
	while (!is_space(s[i]) && s[i])
		i ++;
	s[i] = '\0';
	found = match_char(s);
	return (found);
}

static int	match_word(char *ret)
{
	int		found;
	char	*token;

	found = 0;
	if (!ret)
		return (0);
	token = strtok(ret, " ");
	if (token)
		found = match_first(ret);
	while (token && found == 1)
	{
		found = match_char(skip_space(token));
		token = strtok(NULL, " ");
	}
	if (token && ! found)
	{
		printf("No match for %s\n", token);
		return (0);
	}
	return (found);
}

char	**set_pointers(char *s, char **ret, int size)
{
	int	i;
	char	*token;

	if (!s)
		return (0);
	s = skip_space(s);
	ret[0] = s;
	token = strstr(s, "   ");
	if (token == NULL && str_has_char(s))
		return (ret);
	ret[1] = skip_space(token);
	*token = '\0';
	i = 1;
	while (token != NULL)
	{
		token = strstr(ret[i], "   ");
		if (token != NULL && str_has_char(token))
		{
			ret[i + 1] = skip_space(token);
			*token = '\0';
			i ++;
		}
		else if (token != NULL && !str_has_char(token))
			break ;			
	}
	return (ret);
}

char	*decode_morse(const char *morse_code)
{
	int	size;
	char **ret;
	int		i;
	
	size = 0;
	size = get_size((char *)morse_code); 
	if (!(ret = calloc(size + 1, 1))) 
		return (NULL);
	ret = set_pointers((char *)morse_code, ret, size + 1);
	i = 0;
	while (ret[i])
	{
		match_word(ret[i]);
		i ++;
	}
}	

int	main(void)
{
	char	s[] = "      t  m      bn   nm      ";
	decode_morse(s);
//	int n = strlen();
	printf("%s\n", s);
	return (0);
}
