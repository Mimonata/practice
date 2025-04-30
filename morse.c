/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   morse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spitul <spitul@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:48:10 by spitul            #+#    #+#             */
/*   Updated: 2025/04/29 21:49:31 by spitul           ###   ########.fr       */
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

static bool	str_has_char(const char *s)
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

static int	get_size(const char *str)
{
	int	size;
	char	*token;
	char	*s;

	if (!str)
		return (0);
	s = (char *)str;
	s = skip_space(s);
	token = strstr(s, "   ");
	if (token == NULL && !str_has_char(s))
		return (0);
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

int	get_last(char *msg)
{
	int	i;

	i = 0;
	if (!msg)
		return (0);
	while (msg[i])
		i ++;
	return(i);
}

static int	match_char(char *s, char *msg)
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
		while (morse[i][k] && !found && s[k])
		{
			if (s[k] == morse[i][k] && s[k])
				k ++;
			else if (s[k] != morse[i][k] && s[k])
				break ;
			if (s[k] == 0 && morse[i][k] == 0)
			{
				found = 1;
				strncat(msg, ascii[i], strlen(ascii[i]));
				break ;
			}
		}
		k = 0;
		i ++;
	}
	return (found);
}

static int	match_word(char *ret, char *msg)
{
	int		found;
	char	*token;

	found = 1;
	if (!ret)
		return (0);
	token = strtok(ret, " ");
	while (token && found == 1)
	{
		found = match_char(skip_space(token), msg);
		token = strtok(NULL, " ");
	}
	if (token && !found)
	{
		//printf("No match for %s\n", token);
		return (0);
	}
	return (found);
}

char	**set_pointers(const char *str, char **ret, char **to_free)
{
	int		i;
	char	*token;
	char	*s;

	if (!str)
		return (0);
	s = strdup(str);
	*to_free = s;
	s = skip_space(s);
	if (s == NULL)
		return (NULL);
	ret[0] = s;
	token = strstr(s, "   ");
	if (token == NULL && str_has_char(s))
		return (ret);
	ret[1] = skip_space(token);
	*token = '\0';
	i = 1;
	while (token != NULL && *(ret[i]) != '\0')
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
	// if (*(ret[i]) == '\0')
	// 	ret[i] = NULL;
	return (ret);
}

size_t	get_size_msg(const char *msg)
{
	char	*token;
	char	*cpy;
	size_t	size;

	size = 0;
	if (!msg)
		return (0);
	cpy = strdup(msg);
	token = strtok(cpy, " ");
	while (token)
	{
		size ++;
		token = strtok(NULL, " ");
		if (!str_has_char(token))
			break;
	}
	free (cpy);
	return (size);
}

char	*decode_morse(const char *morse_code)
{
	int		i;
	int		size;
	char	**ret;
	char	*to_free;
	char	*msg;
	
	size = 0;
	size = get_size(morse_code); 
	if (size == 0)
		return (NULL);
	ret = calloc(size + 1, sizeof(char *));
	if (!ret) 
		return (NULL);
	to_free = NULL;
	size += get_size_msg(morse_code);
	if(!(msg = calloc(size, sizeof(char))))
		return (NULL);
	ret = set_pointers(morse_code, ret, &to_free);
	i = 0;
	while (ret[i])
	{
		match_word(ret[i], msg);
		if (ret[i + 1])
			msg[get_last(msg)] = ' ';
		i ++;
	}
	printf("%s\"\n", msg);
	free (ret);
	free (to_free);
	return (msg);
}	

int	main(void)
{
	char	s[] = ".... . -.--   .--- ..- -.. .                  ";
	char	t[] = ".. -.--.- --   -... --- -..-. -- -...- ...- ..   --.. -..- .- -..-. ...-..- ...   ...-- --- ..   .-... -.-.-- ---... .----. ----. --   ...---... .--- .-..-.   ...- -....- ..--.-   .-..";
	char	p[] = "  ...---... -.-.--   - .... .   --.- ..- .. -.-. -.-   -... .-. --- .-- -.   ..-. --- -..-   .--- ..- -- .--. ...   --- ...- . .-.   - .... .   .-.. .- --.. -.--   -.. --- --. .-.-.-                             ";
	
	decode_morse(s);
	decode_morse(t);
	decode_morse(p);
	return (0);
}
