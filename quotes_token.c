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

int	quotes_token_len(const char *s, int i)
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
		else if (quote == s[i + len]
			&& (i + len == 0 || s[i + len - 1] != '\\'))
			quote = 0;
		len++;
	}
	return (len);
}
