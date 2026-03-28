/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 14:40:00 by yurimdm           #+#    #+#             */
/*   Updated: 2026/03/28 01:23:52 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_escaped(const char *str, int i)
{
	int	backslashes;

	backslashes = 0;
	while (i - 1 - backslashes >= 0 && str[i - 1 - backslashes] == '\\')
		backslashes++;
	return ((backslashes % 2) == 1);
}

int	has_unclosed_quotes(const char *str)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (str[i])
	{
		if (!quote && (str[i] == '\'' || str[i] == '"')
			&& !is_escaped(str, i))
			quote = str[i];
		else if (quote == '\'' && str[i] == '\'')
			quote = 0;
		else if (quote == '"' && str[i] == '"'
			&& !is_escaped(str, i))
			quote = 0;
		i++;
	}
	return (quote != 0);
}
