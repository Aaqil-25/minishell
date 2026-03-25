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

static size_t	count_words(const char *s)
{
	size_t	count;
	int		i;
	char	quote_char;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (is_separator(s[i]))
		{
			i++;
			continue;
		}
		count++;
		quote_char = 0;
		while (s[i] && (quote_char || !is_separator(s[i])))
		{
			if (!quote_char && is_quote(s[i]))
				quote_char = s[i];
			else if (quote_char == s[i] && (i == 0 || s[i - 1] != '\\'))
				quote_char = 0;
			i++;
		}
	}
	return (count);
}

static char	*extract_word(const char *s, size_t word_num)
{
	size_t	count;
	int		i;
	char	quote_char;
	int		start;
	size_t	len;
	char	*word;

	count = 0;
	i = 0;
	quote_char = 0;
	while (s[i])
	{
		if (is_separator(s[i]))
		{
			i++;
			continue;
		}
		start = i;
		quote_char = 0;
		while (s[i] && (quote_char || !is_separator(s[i])))
		{
			if (!quote_char && is_quote(s[i]))
				quote_char = s[i];
			else if (quote_char == s[i] && (i == 0 || s[i - 1] != '\\'))
				quote_char = 0;
			i++;
		}
		if (count == word_num)
		{
			len = i - start;
			word = (char *)malloc((len + 1) * sizeof(char));
			if (!word)
				return (NULL);
			ft_strlcpy(word, s + start, len + 1);
			return (word);
		}
		count++;
	}
	return (NULL);
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
	i = 0;
	while (i < word_count)
	{
		result[i] = extract_word(s, i);
		if (!result[i])
		{
			while (i > 0)
			{
				i--;
				free(result[i]);
			}
			free(result);
			return (NULL);
		}
		i++;
	}
	result[word_count] = NULL;
	return (result);
}
