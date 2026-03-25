/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:18:59 by ymazzett         #+#    #+#              */
/*   Updated: 2026/03/25 15:18:59 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			continue ;
		}
		i += quotes_token_len(s, i);
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
			continue ;
		}
		len = quotes_token_len(s, i);
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
