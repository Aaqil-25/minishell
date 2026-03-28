/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymazzett <ymazzett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:18:59 by ymazzett         #+#    #+#              */
/*   Updated: 2026/03/25 15:18:59 by ymazzett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_separator(char c)
{
	return (c == ' ' || c == '	' || c == '\n');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
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

static int	is_escaped(const char *s, int i)
{
	int	backslashes;

	backslashes = 0;
	while (i - 1 - backslashes >= 0 && s[i - 1 - backslashes] == '\\')
		backslashes++;
	return ((backslashes % 2) == 1);
}

int	quotes_token_len(const char *s, int i)
{
	char	quote;
	int		len;

	len = operator_len(s, i);
	if (len)
		return (len);
	quote = 0;
	len = 0;
	while (s[i + len])
	{
		if (!quote && (is_separator(s[i + len])
				|| operator_len(s, i + len)))
			break ;
		if (!quote && is_quote(s[i + len])
			&& !is_escaped(s, i + len))
			quote = s[i + len];
		else if (quote == '\'' && s[i + len] == '\'')
			quote = 0;
		else if (quote == '"' && s[i + len] == '"'
			&& !is_escaped(s, i + len))
			quote = 0;
		len++;
	}
	return (len);
}
