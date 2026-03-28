/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_env_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:01:15 by yurimdm           #+#    #+#             */
/*   Updated: 2026/03/25 20:19:50 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	arg_skip_quote(char *arg, size_t *i, char *quote)
{
	size_t	backslashes;

	backslashes = 0;
	while (*i > backslashes && arg[*i - 1 - backslashes] == '\\')
		backslashes++;
	if (!*quote && (arg[*i] == '\'' || arg[*i] == '"')
		&& ((backslashes % 2) == 0))
	{
		*quote = arg[*i];
		(*i)++;
		return (1);
	}
	if (*quote == '\'' && arg[*i] == '\'')
	{
		*quote = 0;
		(*i)++;
		return (1);
	}
	if (*quote == '"' && arg[*i] == '"' && ((backslashes % 2) == 0))
	{
		*quote = 0;
		(*i)++;
		return (1);
	}
	return (0);
}

static char	*arg_expand_special(char *new_arg, char *arg,
		size_t *i, char **env)
{
	char	*home;
	char	*replacement;

	if (*i == 0 && arg[*i] == '~'
		&& (arg[*i + 1] == '\0' || arg[*i + 1] == '/'))
	{
		home = exec_get_env_value(env, "HOME");
		(*i)++;
		if (!home)
			return (env_append_part(new_arg, "~", 1));
		return (env_append_part(new_arg, home, ft_strlen(home)));
	}
	if (arg[*i] == '~')
	{
		new_arg = env_append_part(new_arg, arg + *i, 1);
		(*i)++;
		return (new_arg);
	}
	replacement = env_substitute(arg, i, env);
	if (!replacement)
		return (free(new_arg), NULL);
	new_arg = env_append_part(new_arg, replacement, ft_strlen(replacement));
	free(replacement);
	return (new_arg);
}

static int	arg_char_escapable(char c)
{
	return (c == '"' || c == '$' || c == '\\' || c == '`' || c == '\n');
}

static char	*arg_append_one_char(char *new_arg, char *arg,
		size_t *i, char quote)
{
	if (quote != '\'' && arg[*i] == '\\' && arg[*i + 1])
	{
		if (!quote || arg_char_escapable(arg[*i + 1]))
		{
			new_arg = env_append_part(new_arg, arg + *i + 1, 1);
			if (!new_arg)
				return (NULL);
			*i += 2;
			return (new_arg);
		}
	}
	new_arg = env_append_part(new_arg, arg + *i, 1);
	if (!new_arg)
		return (NULL);
	(*i)++;
	return (new_arg);
}

char	*parse_arg_by_quote(char *arg, char **env)
{
	char	*new_arg;
	char	quote;
	size_t	i;

	new_arg = ft_strdup("");
	if (!new_arg)
		return (NULL);
	quote = 0;
	i = 0;
	while (arg && arg[i])
	{
		if (arg_skip_quote(arg, &i, &quote))
			continue ;
		if (quote != '\'' && (arg[i] == '$' || (i == 0 && arg[i] == '~')))
		{
			new_arg = arg_expand_special(new_arg, arg, &i, env);
			if (!new_arg)
				return (NULL);
			continue ;
		}
		new_arg = arg_append_one_char(new_arg, arg, &i, quote);
		if (!new_arg)
			return (NULL);
	}
	return (new_arg);
}
