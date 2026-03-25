/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:18:59 by ymazzett          #+#    #+#             */
/*   Updated: 2026/03/25 15:18:59 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_separator(char c)
{
	return (c == ' ' || c == '	' || c == '\n');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

static int	is_operator(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

static int	operator_len(const char *s, int i)
{
	if (s[i] == '|')
		return (1);
	if ((s[i] == '<' || s[i] == '>') && s[i + 1] == s[i])
		return (2);
	if (s[i] == '<' || s[i] == '>')
		return (1);
	return (0);
}

static int	token_len(const char *s, int i)
{
	char	quote;
	int		len;

	if (operator_len(s, i) > 0)
		return (operator_len(s, i));
	quote = 0;
	len = 0;
	while (s[i + len])
	{
		if (!quote && (is_separator(s[i + len]) || is_operator(s[i + len])))
			break ;
		if (!quote && is_quote(s[i + len]))
			quote = s[i + len];
		else if (quote == s[i + len] && (i + len == 0 || s[i + len - 1] != '\\'))
			quote = 0;
		len++;
	}
	return (len);
}

static size_t	count_words(const char *s)
{
	size_t	count;
	int		i;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (is_separator(s[i]))
		{
			i++;
			continue;
		}
		i += token_len(s, i);
		count++;
	}
	return (count);
}

static int	populate_words(char const *s, char **result)
{
	int		i;
	size_t	j;
	int		len;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (is_separator(s[i]))
		{
			i++;
			continue;
		}
		len = token_len(s, i);
		result[j] = ft_substr(s, i, len);
		if (!result[j])
			return (0);
		i += len;
		j++;
	}
	result[j] = NULL;
	return (1);
}

char	**wd_n_quotes_split(char const *s)
{
	char		**result;
	size_t		word_count;
	size_t		i;

	if (!s)
		return (NULL);
	word_count = count_words(s);
	result = (char **)malloc((word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	if (!populate_words(s, result))
	{
		i = 0;
		while (i < word_count)
			free(result[i++]);
		free(result);
		return (NULL);
	}
	return (result);
}
